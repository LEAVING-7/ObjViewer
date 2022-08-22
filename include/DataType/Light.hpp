#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::data {
struct Light {
  alignas(16) glm::vec3 position;
  alignas(16) glm::vec3 color;
};
} // namespace myvk::data