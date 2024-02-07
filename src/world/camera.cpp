module camera;

import app;

Camera::Camera(const glm::vec2 &pos, const float zoom)
    : pos{pos}, zoom{zoom},
      proj{glm::ortho<float>(-App::ASPECT_RATIO, +App::ASPECT_RATIO, -1.0f,
                             +1.0f)} {}

const glm::vec2 &Camera::getPos() const { return pos; }
void Camera::translate(const glm::vec2 &v) { pos += v; }

glm::mat4 Camera::getView() const {
  glm::mat4 out = glm::translate(proj, -glm::vec3{pos, 0});
  out[0][0] *= zoom;
  out[1][1] *= zoom;
  return out;
}
const glm::mat4 &Camera::getProj() const { return proj; }

float Camera::relSpeed() const { return speed * zoom; }
