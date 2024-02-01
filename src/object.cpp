module object;

import debug;

glm::uvec3 Object::getColor() const { return color; }

Object::Object(ngon_opts &&opts, obj_opts &&obj_opts)
    : Object(std::make_unique<Mesh>(
                 Mesh::ngon(opts.sides, opts.radius, opts.offset)),
             std::make_unique<Collider>(
                 Convex::create(*this, opts.sides, opts.radius, opts.offset)),
             std::move(obj_opts)) {
  println("object convex {:x}", reinterpret_cast<size_t>(&*collider));
}
Object::Object(circle_opts &&opts, obj_opts &&obj_opts)
    : Object(std::make_unique<Mesh>(Mesh::circle(opts.radius)),
             std::make_unique<Collider>(Circle::create(*this, opts.radius)),
             std::move(obj_opts)) {}
Object::Object(std::unique_ptr<Mesh> mesh, std::unique_ptr<Collider> collider,
               obj_opts &&opts)
    : mesh{std::move(mesh)}, collider{std::move(collider)}, pos{opts.pos},
      rotation{opts.rotation}, mass{opts.mass}, color{opts.color} {}
Object::~Object() = default;

Object Object::ngon(ngon_opts &&opts, obj_opts &&obj_opts) {
  return {std::move(opts), std::move(obj_opts)};
}
Object Object::circle(circle_opts &&opts, obj_opts &&obj_opts) {
  return {std::move(opts), std::move(obj_opts)};
}

void Object::translate(const glm::vec2 &v) { pos += v; }
void Object::rotate(const float r) { rotation += r; }