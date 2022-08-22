#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "DataType/Mesh.hpp"
#include "EasyVK/BufferAllocator.hpp"
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

   ezvk::AllocatedBuffer allocateVertices( ezvk::BufferAllocator& allocator);
   ezvk::AllocatedBuffer allocateIndices( ezvk::BufferAllocator& allocator);
   ezvk::AllocatedBuffer
  allocateVerticesUsingStaging( ezvk::BufferAllocator& allocator,
                                ezvk::CommandPool& cmdPool, VkDevice device,
                               VkQueue submitQueue);
   ezvk::AllocatedBuffer
  allocateIndicesUsingStaging( ezvk::BufferAllocator& allocator,
                               ezvk::CommandPool& cmdPool, VkDevice device,
                              VkQueue submitQueue);
};
} // namespace myvk::data