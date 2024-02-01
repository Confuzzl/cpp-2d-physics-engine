module scene;

import object;

const glm::vec2 &Scene::Camera::getPos() const { return pos; }
void Scene::Camera::translate(const glm::vec2 &v) { pos += v; }

glm::mat4 Scene::Camera::getView() const {
  glm::mat4 out = glm::translate(proj, -glm::vec3{pos, 0});
  out[0][0] *= zoom;
  out[1][1] *= zoom;
  return out;
}
const glm::mat4 &Scene::Camera::getProj() const { return proj; }
void Scene::Camera::setZoom(const float zoom) { this->zoom = zoom; }

Scene::Camera::Camera(const glm::vec2 &pos, const float zoom)
    : pos{pos}, zoom{zoom} {}

Scene::Scene() {}
Scene::~Scene() = default;