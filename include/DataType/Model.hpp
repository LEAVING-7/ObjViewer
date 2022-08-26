#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "DataType/Mesh.hpp"
#include "DataType/Texture.hpp"
#include "EasyVK/BufferAllocator.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace myvk::data {
// struct VertexInputDescription {
//   std::vector<VkVertexInputBindingDescription>   bindings;
//   std::vector<VkVertexInputAttributeDescription> attributes;
// };
// struct Vertex {
//   glm::vec3 pos;
//   glm::vec3 norm;
//   glm::vec2 uv;

//   static VertexInputDescription GetInputDescription();
// };

// struct Mesh {
//   std::vector<Vertex>       vertices;
//   std::vector<u32>          indices;
//   std::vector<TextureImage> textures;
// };

class ObjModel {
public:
  std::vector<Vertex> vertices;
  std::vector<u32>    indices;

  ObjModel(ccstr filename);

  ObjModel()  = default;
  ~ObjModel() = default;

  ezvk::AllocatedBuffer allocateVertices(ezvk::BufferAllocator& allocator);
  ezvk::AllocatedBuffer allocateIndices(ezvk::BufferAllocator& allocator);
  ezvk::AllocatedBuffer
  allocateVerticesUsingStaging(ezvk::BufferAllocator& allocator,
                               ezvk::CommandPool& cmdPool, VkDevice device,
                               VkQueue submitQueue);
  ezvk::AllocatedBuffer
  allocateIndicesUsingStaging(ezvk::BufferAllocator& allocator,
                              ezvk::CommandPool& cmdPool, VkDevice device,
                              VkQueue submitQueue);
};

// class Model {
//   std::vector<Mesh> meshes;
//   std::string       directory;
//   Model() = default;
//   Model(ccstr path);
//   ~Model() = default;

//   void loadModel();
// };

// class ModelLoader {
//   const aiScene* scene;
//   std::string    path;
//   void create(std::string path, )
// };

} // namespace myvk::data