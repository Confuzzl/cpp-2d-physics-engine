export module object_;

import <glm/glm.hpp>;
// import <glm/gtc/matrix_transform.hpp>;
// import <glm/gtc/type_ptr.hpp>;
// import <glm/gtx/string_cast.hpp>;
// import <glm/gtx/matrix_interpolation.hpp>;

export struct Object {
  double mass;

  glm::vec2 pos{};
  glm::vec2 velocity{};

  double theta = 0;
  double angularVelocty = 0;
};