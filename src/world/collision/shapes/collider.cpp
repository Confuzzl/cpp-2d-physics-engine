module collider;

import object;

import debug;

Collider::Collider(const Object &parent, std::unique_ptr<AABB> aabb)
    : parent{parent}, aabb{std::move(aabb)} {}

const auto &Collider::props() const { return parent.properties; }
glm::vec2 Collider::pos() const { return parent.properties.pos; }
float Collider::rot() const { return parent.properties.rotation; }

AABB &Collider::getAABB() { return *aabb; }
