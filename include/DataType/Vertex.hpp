#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::data {
struct VertexInputDescription {
  std::vector<VkVertexInputBindingDescription>   bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;

  VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static VertexInputDescription GetDescription();
};

} // namespace myvk::data