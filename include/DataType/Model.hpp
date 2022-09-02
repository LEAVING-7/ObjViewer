#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "assimp/ObjMaterial.h"

#include "DataType/Mesh.hpp"
#include "EasyVK/Allocator.hpp"
namespace myvk::data {

class ObjModel {
public:
  std::vector<Vertex> vertices;
  std::vector<u32>    indices;

  ObjModel(ccstr filename);

  ObjModel()  = default;
  ~ObjModel() = default;

  ezvk::AllocatedBuffer allocateVertices(ezvk::Allocator& allocator);
  ezvk::AllocatedBuffer allocateIndices(ezvk::Allocator& allocator);
  ezvk::AllocatedBuffer allocateVerticesUsingStaging(ezvk::Allocator& allocator,
                                                     ezvk::CommandPool& cmdPool,
                                                     VkDevice           device,
                                                     VkQueue submitQueue);
  ezvk::AllocatedBuffer allocateIndicesUsingStaging(ezvk::Allocator& allocator,
                                                    ezvk::CommandPool& cmdPool,
                                                    VkDevice           device,
                                                    VkQueue submitQueue);
};

class Model {

  Model() = default;
  Model(ccstr path);
  ~Model() = default;
};

} // namespace myvk::data