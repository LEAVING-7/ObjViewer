#pragma once
#include "common.hpp"
#include "pch.hpp"

#include <unordered_map>

#include "myvk-bootstrap/BufferAllocator.hpp"
#include "myvk-bootstrap/FrameBuffer.hpp"
#include "myvk-bootstrap/Shader.hpp"
#include "myvk-bootstrap/Swapchain.hpp"
#include "myvk-bootstrap/SyncStructures.hpp"

namespace myvk_bs {
class Application;
class Device;
struct GraphicPipelineBuilder;

class Renderer {
public:
  Renderer(Application* app);
  ~Renderer();

public:
  void create();
  void destroy();

  void initialize();
  void prepare();
  void render();

  void createWindow(u32 width = 800, u32 height = 600);
  void destroyWindow();
  bool windowShouldClose();

  void createSwapchain();
  void destroySwapchain();

private:
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

  // TODO remove temp function

  void createCommand();
  void destroyCommand();

public:
  VkExtent2D   m_windowExtent;
  GLFWwindow*  m_window;
  VkSurfaceKHR m_surface;

  VkCommandPool   m_cmdPool;
  VkFormat        m_depthImageFormat;
  AllocatedImage  m_depthImage;
  VkImageView     m_depthImageView;
  VkCommandBuffer m_cmdVertexBuffer;

  VkRenderPass m_renderPass;

  Framebuffer m_frameBuffer;

  std::unique_ptr<GraphicPipelineBuilder> m_defaultPipelineBuilder;
  VkPipeline                              m_defaultPipeline;
  VkPipelineLayout                        m_defaultPipelineLayout;

  VkQueue         m_graphicQueue;
  u32             m_graphicQueueIndex;
  VkCommandPool   m_mainCommandPool;
  VkCommandBuffer m_mainCommandBuffer;

  Semaphore m_presentSemaphore, m_renderSemaphore;
  Fence     m_renderFence;

private:
  Application*               m_application;
  std::unique_ptr<Swapchain> m_swapchainObj;

  std::unordered_map<std::string, Shader> m_shaders;
};
} // namespace myvk_bs