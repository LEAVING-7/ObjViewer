#pragma once
#include "common.hpp"
#include "pch.hpp"

#include <unordered_map>

#include "DataType/Camera.hpp"
#include "GUI/MainWindow.hpp"

#include "bootstrap/BufferAllocator.hpp"
#include "bootstrap/Descriptor.hpp"
#include "bootstrap/FrameBuffer.hpp"
#include "bootstrap/GraphicPipelineBuilder.hpp"
#include "bootstrap/Shader.hpp"
#include "bootstrap/Swapchain.hpp"
#include "bootstrap/SyncStructures.hpp"

namespace myvk::bs {
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
  // TODO: remove this later
  void createMesh();
  void destroyMesh();

  void createDescriptorSets();
  void destroyDescriptorSets();

public:
  RendererState m_state;

  gui::MainWindow m_window;

  VkSurfaceKHR m_surface;

  VkFormat       m_depthImageFormat;
  AllocatedImage m_depthImage;
  VkImageView    m_depthImageView;

  VkRenderPass m_renderPass;

  Framebuffer m_frameBuffer;

  std::unique_ptr<GraphicPipelineBuilder> m_defaultPipelineBuilder;
  VkPipeline                              m_defaultPipeline;
  VkPipelineCache                         m_defaultPipelineCache;
  VkPipelineLayout                        m_defaultPipelineLayout;

  VkPipeline m_axisPipeline;
  

  VkQueue m_graphicQueue;
  u32     m_graphicQueueIndex;
  VkQueue m_transferQueue;
  u32     m_transferQueueIndex;

  DescriptorPool               m_descPool;
  DescriptorSetLayout          m_uniformLayout;
  std::vector<VkDescriptorSet> m_uniformSets;

  AllocatedBuffer m_testMesh;
  AllocatedBuffer m_testIndex;
  AllocatedBuffer m_uniformBuffer;
  // private:
  Application*               m_application;
  std::unique_ptr<Swapchain> m_swapchainObj;

  std::unordered_map<std::string, Shader> m_shaders;
};
} // namespace myvk::bs