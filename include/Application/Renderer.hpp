#pragma once
#include "common.hpp"
#include "pch.hpp"

#include <unordered_map>

#include "DataType/Camera.hpp"
#include "DataType/Model.hpp"
#include "DataType/Texture.hpp"
#include "GUI/MainWindow.hpp"

#include "EasyVK/BufferAllocator.hpp"
#include "EasyVK/Descriptor.hpp"
#include "EasyVK/FrameBuffer.hpp"
#include "EasyVK/PipelineBuilder.hpp"
#include "EasyVK/Shader.hpp"
#include "EasyVK/Swapchain.hpp"
#include "EasyVK/SyncStructures.hpp"

namespace myvk {
class Application;
class Device;

struct RendererState {
  data::Camera camera{};
};

class Renderer {
public:
  void create(Application* app);
  void destroy();

  void initialize();
  void prepare();
  void render();

  void createWindow(VkInstance instance, u32 width = 800, u32 height = 600);
  void destroyWindow(VkInstance instance);
  bool windowShouldClose();

  void createSwapchain();
  void destroySwapchain();

  void createDepthImages();
  void destroyDepthImages();

  void createRenderPass(bool includeDepth, bool clear = true);
  void destroyRenderPass();

  void createFrameBuffer(bool includeDepth);
  void destroyFrameBuffer();

  void createShaders();
  void destroyShaders();

  void createDefaultPipeline();
  void destroyDefaultPipeline();

  void getGraphicQueueAndQueueIndex();

  void recreateSwapchain();

  void createMesh();
  void destroyMesh();

  void createDescriptorSets();
  void destroyDescriptorSets();

  void createTextures();
  void destroyTextures();

public:
  RendererState m_state;

  gui::MainWindow m_window;

  VkSampleCountFlagBits m_sampleCount = VK_SAMPLE_COUNT_4_BIT;

  VkSurfaceKHR m_surface;

  VkFormat             m_depthImageFormat;
  ezvk::AllocatedImage m_depthImage;
  VkImageView          m_depthImageView;

  ezvk::AllocatedImage m_resolveImage;
  VkImageView          m_resolveView;

  VkRenderPass m_renderPass;

  ezvk::Framebuffer m_frameBuffer;

  std::unique_ptr<ezvk::GraphicPipelineBuilder> m_defaultPipelineBuilder;
  VkPipeline                                    m_defaultPipeline;
  VkPipelineCache                               m_defaultPipelineCache;
  VkPipelineLayout                              m_defaultPipelineLayout;

  ezvk::CommandPool m_transientCmdPool;

  VkQueue m_graphicQueue;
  u32     m_graphicQueueIndex;

  ezvk::DescriptorPool         m_descPool;
  ezvk::DescriptorSetLayout    m_uniformLayout;
  std::vector<VkDescriptorSet> m_uniformSets;

  data::TextureImage m_testTexture;
  ezvk::ImageView    m_testTextureImageView;
  ezvk::Sampler      m_testTextureSampler;

  data::ObjModel        m_testModel;
  ezvk::AllocatedBuffer m_testModelVertexBuf;
  ezvk::AllocatedBuffer m_testModelIndexBuf;

  ezvk::AllocatedBuffer m_uniformBuffer;
  ezvk::AllocatedBuffer m_lightBuffer;

  // private:
  Application*                     m_application;
  std::unique_ptr<ezvk::Swapchain> m_swapchainObj;

  std::unordered_map<std::string, ezvk::Shader> m_shaders;
};
} // namespace myvk