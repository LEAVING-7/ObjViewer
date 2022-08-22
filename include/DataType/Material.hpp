#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::data {
struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float     shininess;
};
} // namespace myvk::data