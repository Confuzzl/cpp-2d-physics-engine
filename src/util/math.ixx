export module math;

import <random>;
import glm;

std::random_device rd;
std::mt19937 mt{rd()};

// export template <typename T = int> T randomInt(T a, T b) {}
export template <typename T = int> T randomInt(const T a, const T b) {
  return std::uniform_int_distribution<T>{a, b}(mt);
}

export glm::u8vec3 randomColor() {
  return {randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)};
}

import <limits>;

export constexpr float F_INF_POS = std::numeric_limits<float>::infinity();
export constexpr float F_INF_NEG = -F_INF_POS;
export constexpr float F_NAN = std::numeric_limits<float>::quiet_NaN();

export constexpr double D_INF_POS = std::numeric_limits<double>::infinity();
export constexpr double D_INF_NEG = -D_INF_POS;
export constexpr double D_NAN = std::numeric_limits<double>::quiet_NaN();