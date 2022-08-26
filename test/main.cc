#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cassert>
#include <cstdio>
#include <filesystem>

std::string path = "assets/crucible/scene.gltf";

void processMesh(aiMesh* mesh, const aiScene* scene) {
  printf("vertex size: %u\n", mesh->mNumVertices);
  printf("material Idx: %u\n", mesh->mMaterialIndex);
}

void processNode(aiNode* node, const aiScene* scene) {
  puts("child");
  for (unsigned i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    printf("mesh address: %p\n", mesh);
    // pushback mesh
  }

  for (unsigned i = 0; i < node->mNumChildren; ++i) {
    processNode(node->mChildren[i], scene);
  }
}

int main() {
  Assimp::Importer importer;
  printf("%ws\n", std::filesystem::current_path().c_str());

  auto scene = importer.ReadFile(path, aiProcess_Triangulate);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    puts("err");
  }
  printf("mesh count: %u\n", scene->mNumMeshes);
  printf("material count: %u\n", scene->mNumMaterials);

  auto directory = path.substr(0, path.find_last_of('/'));
  printf("%s\n", directory.c_str());
  for(unsigned i = 0; i< scene->mNumMaterials; ++i) {
    printf("material: %s\n", scene->mMaterials[i]->GetName().C_Str());
  }

  aiMaterial m;
  processNode(scene->mRootNode, scene);

  assert(scene != nullptr);
}