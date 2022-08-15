#pragma once
#include "common.hpp"
#include "pch.hpp"

#include <unordered_map>

#include "GUI/MainWindow.hpp"
#include "bootstrap/BufferAllocator.hpp"
#include "bootstrap/FrameBuffer.hpp"
#include "bootstrap/Shader.hpp"
#include "bootstrap/Swapchain.hpp"
#include "bootstrap/SyncStructures.hpp"
#include "bootstrap/GraphicPipelineBuilder.hpp"

namespace myvk::bs {
class Application;
class Device;

struct GPUPushConstant {
  glm::vec4 color;
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

public:
  gui::MainWindow m_window;

  VkSurfaceKHR m_surface;

  VkFormat       m_depthImageFormat;
  AllocatedImage m_depthImage;
  VkImageView    m_depthImageView;

  VkRenderPass m_renderPass;

  Framebuffer m_frameBuffer;

  std::unique_ptr<GraphicPipelineBuilder> m_defaultPipelineBuilder;
  VkPipeline                              m_defaultPipeline;
  VkPipelineLayout                        m_defaultPipelineLayout;

  VkQueue m_graphicQueue;
  u32     m_graphicQueueIndex;

// private:
  Application*               m_application;
  std::unique_ptr<Swapchain> m_swapchainObj;

  std::unordered_map<std::string, Shader> m_shaders;
};
} // namespace myvk::bs