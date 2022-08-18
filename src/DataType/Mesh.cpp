#include "DataType/Mesh.hpp"

namespace myvk::data {
VertexInputDescription Vertex::GetDescription() {
  VertexInputDescription ret;

  VkVertexInputBindingDescription binding{
      .binding   = 0,
      .stride    = sizeof(Vertex),
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };

  ret.bindings.push_back(binding);

  VkVertexInputAttributeDescription posAttr{
      .location = 0,
      .binding  = 0,
      .format   = VK_FORMAT_R32G32B32_SFLOAT,
      .offset   = offsetof(Vertex, pos),
  };

  VkVertexInputAttributeDescription colorAttr{
      .location = 1,
      .binding  = 0,
      .format   = VK_FORMAT_R32G32B32_SFLOAT,
      .offset   = offsetof(Vertex, color),
  };

  VkVertexInputAttributeDescription normAttr{
      .location = 2,
      .binding  = 0,
      .format   = VK_FORMAT_R32G32B32_SFLOAT,
      .offset   = offsetof(Vertex, norm),
  };

  VkVertexInputAttributeDescription uvAttr{
      .location = 3,
      .binding  = 0,
      .format   = VK_FORMAT_R32G32_SFLOAT,
      .offset   = offsetof(Vertex, uv),
  };

  ret.attributes.push_back(normAttr);
  ret.attributes.push_back(posAttr);
  ret.attributes.push_back(colorAttr);
  ret.attributes.push_back(uvAttr);
  return ret;
}

} // namespace myvk::data

bool operator==(const myvk::data::Vertex& lhs, const myvk::data::Vertex& rhs) {
  if (&lhs == &rhs) {
    return true;
  } else {
    return lhs.pos == rhs.pos && lhs.color == rhs.color && lhs.uv == rhs.uv;
  }
}