#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "DataType/Mesh.hpp"
#include "bootstrap/BufferAllocator.hpp"
namespace myvk::data {
class ObjModel {
public:
  std::vector<Vertex> vertices;
  std::vector<u32>    indices;

  ObjModel(ccstr filename);

  ObjModel()                           = default;
  ObjModel(const ObjModel&)            = default;
  ObjModel(ObjModel&&)                 = default;
  ObjModel& operator=(const ObjModel&) = default;
  ObjModel& operator=(ObjModel&&)      = default;
  ~ObjModel()                          = default;

  bs::AllocatedBuffer allocateVertices(bs::BufferAllocator& allocator);
  bs::AllocatedBuffer allocateIndices(bs::BufferAllocator& allocator);
  bs::AllocatedBuffer
  allocateVerticesUsingStaging(bs::BufferAllocator& allocator,
                               bs::CommandPool& cmdPool, VkDevice device,
                               VkQueue submitQueue);
  bs::AllocatedBuffer
  allocateIndicesUsingStaging(bs::BufferAllocator& allocator,
                              bs::CommandPool& cmdPool, VkDevice device,
                              VkQueue submitQueue);
};
} // namespace myvk::data