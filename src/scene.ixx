export module scene;

import glm;

import <set>;

import rendering;
import <memory>;

export struct Object;

export struct Scene {
  struct Camera {
    glm::vec2 pos{};

    float zoom = 1;
    glm::mat4 proj = glm::ortho<float>(-Renderer::ASPECT_RATIO,
                                       +Renderer::ASPECT_RATIO, -1.0f, +1.0f);

    const glm::vec2 &getPos() const;
    void translate(const glm::vec2 &v);

    glm::mat4 getView() const;
    const glm::mat4 &getProj() const;
    void setZoom(const float zoom);

    Camera(const glm::vec2 &pos = {}, const float zoom = 1);
  } camera{{0, 0}, 1};

  std::set<std::unique_ptr<Object>> objects;

  Scene();
  ~Scene();
} MAIN_SCENE{};