module collider;

import debug;

Collider::Collider(aabb_t &&aabb, const glm::vec2 &pos, const float rot)
    : aabb{std::move(aabb)} {
  setPos(pos);
  setRot(rot);
}

glm::vec2 Collider::pos() const { return position; }
void Collider::translate(const glm::vec2 &v) {}
void Collider::setPos(const glm::vec2 &v) {}
float Collider::rot() const { return rotation; }
void Collider::rotate(const float r) {}
void Collider::setRot(const float r) {}
