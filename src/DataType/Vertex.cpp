#include "DataType/Vertex.hpp"

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

  VkVertexInputAttributeDescription texCoordAttr{
      .location = 2,
      .binding  = 0,
      .format   = VK_FORMAT_R32G32_SFLOAT,
      .offset   = offsetof(Vertex, texCoord),
  };

  ret.attributes.push_back(posAttr);
  ret.attributes.push_back(colorAttr);
  ret.attributes.push_back(texCoordAttr);
  return ret;
}

} // namespace myvk::data