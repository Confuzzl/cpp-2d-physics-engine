module;

#include "util/gl.h"
#include "util/main_objects.h"

module world_frame;

import app;
import aabb;
import camera;
import mesh;
import glm;
import vertex_layout;
import bo_heap;
import shaders;
import math;
import ecs_component;
import <random>;
import debug;
import ubo;
import texture;
import fbo;
import <ranges>;
import quadtree;

static glm::vec4 offsets() {
  return {random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f),
          random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f)};
}
static glm::vec2 point(const unsigned int i) {
  static glm::vec4 o[]{offsets(), offsets(), offsets(), offsets()};
  const double t = glfwGetTime();
  return {sin(o[i][0] * t + o[i][1]), sin(o[i][2] * t + o[i][3])};
}

static auto quadrants(const BoundingBox &nodeBox) {
  struct Boxes {
    BoundingBox tlBox, trBox, blBox, brBox;
  };
  const auto halfBounds = nodeBox.size() / 2.0f;
  const glm::vec2 starts[4] = {
      nodeBox.min, nodeBox.min + glm::vec2{halfBounds.x, 0},
      nodeBox.min + glm::vec2{0, halfBounds.y}, nodeBox.min + halfBounds};
  return Boxes{.tlBox = BoundingBox::startSize(starts[2], halfBounds),
               .trBox = BoundingBox::startSize(starts[3], halfBounds),
               .blBox = BoundingBox::startSize(starts[0], halfBounds),
               .brBox = BoundingBox::startSize(starts[1], halfBounds)};
}

static void drawNode(const WorldFrame *self, const collision::Quadtree &tree,
                     const collision::Quadtree::Node &node,
                     const BoundingBox &box) {
  self->drawBoxConstant(box, 0.01f, RED);
  if (node.isLeaf())
    return;
  const auto [tlBox, trBox, blBox, brBox] = quadrants(box);
  drawNode(self, tree, tree.nodes[node.first + 0], tlBox);
  drawNode(self, tree, tree.nodes[node.first + 1], trBox);
  drawNode(self, tree, tree.nodes[node.first + 2], blBox);
  drawNode(self, tree, tree.nodes[node.first + 3], brBox);
}

void WorldFrame::render() {
  matrix = MAIN_CAMERA.getView();

  shaders::getUBO<shaders::uniform::ViewBlock>().update({matrix});

  drawGrid();

  // for (const auto [id, rend] : ecs.viewComponents<ecs::Renderable>()) {
  //   drawMesh(rend->mesh);
  // }

  // const auto corner = glm::vec2{App::ASPECT_RATIO, 1.0} / MAIN_CAMERA.zoom();
  // const auto screen = BoundingBox{-corner, corner} + MAIN_CAMERA.getPos();

  drawNode(this, MAIN_SCENE.data, MAIN_SCENE.data.nodes[0],
           MAIN_SCENE.data.BOUNDS);

  using namespace ecs::comp;

  for (const auto [id, t, p, s, b] :
       ECS.viewIDComp<Transformable, Physical, Shapeable, Boundable>()) {
    const auto &trans = *t;
    const auto &shape = *s;
    const auto &bound = *b;

    using namespace collision;
    const auto color = colors::random_i(id);
    shape.visit(
        [this, &trans, color](const Wall &wall) {
          const auto [a, b] = wall.getVertices(trans);
          glm::vec2 points[2]{a, b};
          drawLinePerspective({points[0], points[1]}, 0.02f, color);
          const glm::vec2 mid = (points[0] + points[1]) / 2.0f;
          const glm::vec2 norm = wall.normal(trans) * 0.1f;
          if (wall.bidirectional)
            drawLineConstant({mid - norm, mid + norm}, 0.01f, color);
          else
            drawLineConstant({mid, mid + norm}, 0.01f, color);
        },
        [this, &trans, color](const Circle &circle) {
          drawCircle(trans.position, circle.radius, color);
        },
        [this, &trans, color](const Polygon &polygon) {});

    const auto &phys = *p;
    drawLineConstant({trans.position, trans.position + phys.linear.velocity},
                     0.03f, BLACK);
    drawLineConstant({trans.position, trans.position + phys.linear.velocity},
                     0.01f, RED);

    const auto angle =
        glm::vec2{std::cos(trans.rotation), std::sin(trans.rotation)} * 0.2f;
    drawLineConstant({trans.position, trans.position + angle}, 0.03f, BLACK);
    drawLineConstant({trans.position, trans.position + angle}, 0.01f, BLUE);

    drawDot(trans.position, 0.03f, BLACK);
    drawDot(trans.position, 0.02f, color);

    drawBox(bound + trans.position, 3, color);

    drawGLPoint(MAIN_APP.cursorWorldPosition(), 10, BLUE);
  }

  for (const auto [draw] : ECS.viewComp<DirectRenderable>()) {
    draw->draw(this);
  }
}

void WorldFrame::drawGrid() const {
  static constexpr unsigned int GRID_X_RADIUS = 4;
  static constexpr unsigned int GRID_Y_RADIUS = 2;

  static constexpr unsigned int GRID_RADIUS = 4;
  static constexpr float MAJOR_LINE_SPACING = 1.0f;
  static constexpr unsigned int MINOR_PER_MAJOR = 4;
  static constexpr float MINOR_LINE_SPACING =
      MAJOR_LINE_SPACING / MINOR_PER_MAJOR;
  static constexpr int NUM_HALF_MAJOR_LINES =
      static_cast<int>(GRID_RADIUS / MAJOR_LINE_SPACING);
  static constexpr int NUM_HALF_MINOR_LINES =
      NUM_HALF_MAJOR_LINES *
      (MINOR_PER_MAJOR - 1); // skip minor that overlaps major
  static VBOHandle MAJOR = VBO_HOLDER.get(NUM_HALF_MAJOR_LINES * 8);
  static VBOHandle MINOR = VBO_HOLDER.get(NUM_HALF_MINOR_LINES * 8);

  const float zoomLevel = MAIN_CAMERA.zoomExponent();

  const glm::vec2 pos = MAIN_CAMERA.getPos();
  const glm::vec2 ipos = {static_cast<int>(pos.x * zoomLevel) / zoomLevel,
                          static_cast<int>(pos.y * zoomLevel) / zoomLevel};

  const float majorSpacing = MAJOR_LINE_SPACING / zoomLevel;
  const float minorSpacing = MINOR_LINE_SPACING / zoomLevel;

  for (auto i = 0; i < NUM_HALF_MAJOR_LINES; i++) {
    const float min = -NUM_HALF_MAJOR_LINES * majorSpacing,
                max = +NUM_HALF_MAJOR_LINES * majorSpacing;
    const float majorGap = i * majorSpacing;

    for (auto j = 1; j < MINOR_PER_MAJOR; j++) {
      const float minorGap = j * minorSpacing;
      const float gap = majorGap + minorGap;
      const glm::vec2 minors[8]{{min, +gap}, {max, +gap}, {min, -gap},
                                {max, -gap}, {+gap, min}, {+gap, max},
                                {-gap, min}, {-gap, max}};
      for (const glm::vec2 p : minors)
        MINOR->write(ipos + p);
    }

    const glm::vec2 majors[8]{
        {min, +majorGap}, {max, +majorGap}, {min, -majorGap}, {max, -majorGap},
        {+majorGap, min}, {+majorGap, max}, {-majorGap, min}, {-majorGap, max}};
    for (const glm::vec2 p : majors)
      MAJOR->write(ipos + p);
  }
  glLineWidth(1);
  SHADERS.basic.setFragColor(LIGHTEST_GRAY).draw(GL_LINES, MINOR);
  glLineWidth(2);
  SHADERS.basic.setFragColor(GRAY).draw(GL_LINES, MAJOR);
  // SHADERS.line.setFragColor(LIGHTEST_GRAY)
  //     .setThickness(0.002f / MAIN_CAMERA.zoom())
  //     .draw(GL_LINES, MINOR);
  // SHADERS.line.setFragColor(GRAY)
  //    .setThickness(0.005f / MAIN_CAMERA.zoom())
  //    .draw(GL_LINES, MAJOR);

  const glm::vec2 axes[4]{{-NUM_HALF_MAJOR_LINES * majorSpacing + ipos.x, 0},
                          {+NUM_HALF_MAJOR_LINES * majorSpacing + ipos.x, 0},
                          {0, -NUM_HALF_MAJOR_LINES * majorSpacing + ipos.y},
                          {0, +NUM_HALF_MAJOR_LINES * majorSpacing + ipos.y}};
  VBO_4->write(axes);
  glLineWidth(4);
  SHADERS.basic.setFragColor(BLACK).draw(GL_LINES, VBO_4);
  // SHADERS.line.setFragColor(BLACK)
  //     .setThickness(0.01f / MAIN_CAMERA.zoom())
  //     .draw(GL_LINES, VBO_4);
}

void WorldFrame::drawBezier(const Bezier &curve, const Color c0, const Color c1,
                            const float thickness, const bool debug) {
  static constexpr auto step = [](const float zoom, const glm::vec2 size) {
    const float s = std::max(size.x, size.y);
    return static_cast<unsigned int>(
        std::max(1.0f, s * std::min(std::sqrt(zoom), 2.0f) * 15.0f));
  };

  BaseFrame::drawBezier(curve, c0, c1, thickness, step, true, debug);
}
void WorldFrame::drawBezier(const Bezier &curve, const Color color,
                            const float thickness, const bool debug) {
  drawBezier(curve, color, color, thickness, debug);
}