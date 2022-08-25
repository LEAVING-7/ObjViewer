#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "glm/gtx/hash.hpp"
namespace myvk::data {
struct VertexInputDescription {
  std::vector<VkVertexInputBindingDescription>   bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec3 norm;
  glm::vec2 uv;

  static VertexInputDescription GetDescription();

  inline bool operator==(const Vertex& other) const {
    if (this == &other)
      return true;
    else
      return pos == other.pos && color == other.color && uv == other.uv;
  }
};
  
} // namespace myvk::data

// data::Vertex hash support
namespace std {
template <> struct hash<myvk::data::Vertex> {
  size_t operator()(myvk::data::Vertex const& vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.uv) << 1);
  }
};
} // namespace std