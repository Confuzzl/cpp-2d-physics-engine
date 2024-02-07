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

export constexpr float POS_INF = std::numeric_limits<float>::infinity();
export constexpr float NEG_INF = -POS_INF;