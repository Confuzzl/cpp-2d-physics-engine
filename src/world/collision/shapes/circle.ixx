export module circle;

import collider;
import query_info;
import glm;

export struct Polygon;

export struct Circle : public Collider {
  const float radius = 1;

private:
  friend std::unique_ptr<Circle>
  std::make_unique<Circle, const Object &, const float &>(const Object &,
                                                          const float &);
  Circle(const Object &parent, const float radius);

public:
  static std::unique_ptr<Circle> create(const Object &parent,
                                        const float radius);

  bool contains(const glm::vec2 &point) const;

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Polygon &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};