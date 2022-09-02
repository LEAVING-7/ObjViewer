#include "DataType/Model.hpp"
#include <string>
#include <unordered_map>
namespace myvk::data {
ObjModel::ObjModel(ccstr filename) {
  using namespace tinyobj;
  attrib_t                attrib;
  std::string             warn, err;
  std::vector<shape_t>    shapes;
  std::vector<material_t> material;
  bool result = LoadObj(&attrib, &shapes, &material, &warn, &err, filename);

  if (!warn.empty()) {
    LOG_WARN("{}", warn);
  }
  if (!err.empty()) {
    LOG_ERR("{}", err);
  }
  if (!result) {
    exit(-1);
  }

  std::unordered_map<Vertex, u32> uniqueVertices{};

  for (auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      Vertex vertex;

      vertex.pos = {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2],
      };

      if (index.texcoord_index > 0)
        vertex.uv = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            1 - attrib.texcoords[2 * index.texcoord_index + 1],
        };

      if (index.normal_index > 0)
        vertex.norm = {
            attrib.normals[3 * index.normal_index + 0],
            attrib.normals[3 * index.normal_index + 1],
            attrib.normals[3 * index.normal_index + 2],
        };

      vertex.color = {1.f, 1.f, 1.f};

      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] = static_cast<u32>(vertices.size());
        vertices.push_back(vertex);
      }

      indices.push_back(uniqueVertices[vertex]);
    }
  }
}

ezvk::AllocatedBuffer ObjModel::allocateVertices(ezvk::Allocator& allocator) {
  ezvk::AllocatedBuffer ret = allocator.createBuffer(
      vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
  ret.transferMemory(allocator, (void*)vertices.data(), ret.size);
  return ret;
}
ezvk::AllocatedBuffer ObjModel::allocateIndices(ezvk::Allocator& allocator) {
  ezvk::AllocatedBuffer ret;
  ret = allocator.createBuffer(indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VMA_MEMORY_USAGE_CPU_TO_GPU);
  ret.transferMemory(allocator, (void*)indices.data(), ret.size);
  return ret;
}

ezvk::AllocatedBuffer
ObjModel::allocateVerticesUsingStaging(ezvk::Allocator&   allocator,
                                       ezvk::CommandPool& cmdPool,
                                       VkDevice device, VkQueue submitQueue) {
  ezvk::AllocatedBuffer stagingBuf = allocator.createBuffer(
      vertices, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
  stagingBuf.transferMemory(allocator, (void*)vertices.data(), stagingBuf.size);

  ezvk::AllocatedBuffer retBuffer = allocator.createBuffer(
      vertices,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VMA_MEMORY_USAGE_GPU_ONLY);
  stagingBuf.copyTo(retBuffer, device, cmdPool, submitQueue);

  allocator.destroyBuffer(stagingBuf);
  return retBuffer;
}
ezvk::AllocatedBuffer
ObjModel::allocateIndicesUsingStaging(ezvk::Allocator&   allocator,
                                      ezvk::CommandPool& cmdPool,
                                      VkDevice device, VkQueue submitQueue) {

  ezvk::AllocatedBuffer stagingBuf = allocator.createBuffer(
      indices, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
  stagingBuf.transferMemory(allocator, (void*)indices.data(), stagingBuf.size);

  ezvk::AllocatedBuffer retBuffer = allocator.createBuffer(
      indices,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VMA_MEMORY_USAGE_GPU_ONLY);

  stagingBuf.copyTo(retBuffer, device, cmdPool, submitQueue);

  allocator.destroyBuffer(stagingBuf);
  return retBuffer;
}
} // namespace myvk::data