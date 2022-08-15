#include "bootstrap/Renderer.hpp"

#include "DataType/Vertex.hpp"
#include "bootstrap/Application.hpp"

namespace myvk::bs {

const std::vector<data::Vertex> g_vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                       int mods) {
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void windowCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {}

void windowFramebufferResizeCallback(GLFWwindow* window, int width,
                                     int height) {
  Renderer* renderer =
      reinterpret_cast<Renderer*>(gui::MainWindow::getUserPointer(window));
  renderer->m_window.m_framebufferResized = true;
}

void Renderer::create(Application* app) {
  m_application = app;
  getGraphicQueueAndQueueIndex();
  // createDepthImages();
  createSwapchain();
  createRenderPass(true);
  createShaders();
  createDefaultPipeline();
  createFrameBuffer(true);

  createMesh();
}

void Renderer::destroy() {
  vkDeviceWaitIdle(*m_application);

  destroyMesh();

  destroyFrameBuffer();
  destroyDefaultPipeline();
  destroyShaders();
  destroyRenderPass();
  destroySwapchain();
  // destroyDepthImages();
}

void Renderer::recreateSwapchain() {
  vkDeviceWaitIdle(*m_application);
  auto [width, height] = m_window.getFrameBufferSize();
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(m_window.m_window, &width, &height);
    gui::MainWindow::waitEvents();
  }

  destroyFrameBuffer();
  destroyDefaultPipeline();
  destroyShaders();
  destroyRenderPass();
  destroySwapchain();
  destroyDepthImages();

  m_window.updateExtent();

  createDepthImages();
  createSwapchain();
  createRenderPass(true);
  createShaders();
  createDefaultPipeline();
  createFrameBuffer(true);
}

void Renderer::prepare() {}

void Renderer::render() {
  glfwPollEvents();
  VkResult result;

  auto& currentData = m_frameBuffer.currentFrameData();

  vkWaitForFences(*m_application, 1, &currentData.renderFence, true,
                  std::numeric_limits<u64>::max());

  u32 swapchainImgIdx;

  result = m_frameBuffer.acquireNextImage(*m_application, *m_swapchainObj,
                                          std::numeric_limits<u64>::max(),
                                          &swapchainImgIdx);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      m_window.isFramebufferResized()) {
    recreateSwapchain();
    m_window.m_framebufferResized = false;
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    LOG_ERR("failed to acquire next image");
    return;
  }

  currentData.renderFence.reset(*m_application);
  VkClearValue colorClear{
      .color = {{0.5f, 0.3f, 0.4f, 1.f}},
  };

  VkClearValue depthClear{
      .depthStencil = {.depth = 1.f},
  };

  VkClearValue clearValue[2] = {colorClear, depthClear};

  VkRenderPassBeginInfo renderPassBI{
      .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .pNext       = nullptr,
      .renderPass  = m_renderPass,
      .framebuffer = m_frameBuffer.framebuffers[swapchainImgIdx],
      .renderArea =
          {
              .offset = {0, 0},
              .extent = {m_window.m_width, m_window.m_height},
          },
      .clearValueCount = 2,
      .pClearValues    = clearValue,
  };
  currentData.cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

  currentData.cmdBuffer.beginRenderPass(&renderPassBI,
                                        VK_SUBPASS_CONTENTS_INLINE);

  currentData.cmdBuffer.bindPipelineGraphic(m_defaultPipeline);

  // currentData.cmdBuffer.bindVertexBuffer(&m_testMesh.buffer);
  currentData.cmdBuffer.draw(3, 1, 0, 0);

  currentData.cmdBuffer.endRenderPass();
  currentData.cmdBuffer.end();

  VkPipelineStageFlags waitStage =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submitInfo{
      .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .pNext                = nullptr,
      .waitSemaphoreCount   = 1,
      .pWaitSemaphores      = &currentData.presentSemaphore,
      .pWaitDstStageMask    = &waitStage,
      .commandBufferCount   = 1,
      .pCommandBuffers      = &currentData.cmdBuffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores    = &currentData.renderSemaphore,
  };

  vkQueueSubmit(m_graphicQueue, 1, &submitInfo, currentData.renderFence);

  VkPresentInfoKHR presentInfo{
      .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .pNext              = nullptr,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores    = &currentData.renderSemaphore,
      .swapchainCount     = 1,
      .pSwapchains        = &m_swapchainObj->m_swapchain.swapchain,
      .pImageIndices      = &swapchainImgIdx,
  };

  vkQueuePresentKHR(m_graphicQueue, &presentInfo);
  m_frameBuffer.frameCount++;
  // m_frameBuffer.updateFrameCount();
}

bool Renderer::windowShouldClose() {
  return m_window.shouldClose();
}

void Renderer::createWindow(VkInstance instance, u32 width, u32 height) {
  m_window.create(width, height, "This is a title", nullptr);
  m_surface = m_window.createSurface(instance);
  LOG_INFO("create surface: {}", (void*)m_surface);

  m_window.setUserPointer(m_window.m_window, this);

  m_window.setErrorCallback()
      .setCursorPosCallback(windowCursorPosCallback)
      .setKeyCallback(windowKeyCallback)
      .setFramebufferSizeCallback(windowFramebufferResizeCallback);
}
void Renderer::destroyWindow(VkInstance instance) {
  vkDestroySurfaceKHR(instance, m_surface, nullptr);
  m_window.destroy();
}

void Renderer::createDepthImages() {
  VkExtent3D imageExtent{
      .width  = m_window.m_width,
      .height = m_window.m_height,
      .depth  = 1,
  };

  m_depthImageFormat = VK_FORMAT_D32_SFLOAT;

  VkImageCreateInfo depthImageCI{
      .sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext       = nullptr,
      .imageType   = VK_IMAGE_TYPE_2D,
      .format      = m_depthImageFormat,
      .extent      = imageExtent,
      .mipLevels   = 1,
      .arrayLayers = 1,
      .samples     = VK_SAMPLE_COUNT_1_BIT,
      .tiling      = VK_IMAGE_TILING_OPTIMAL,
      .usage       = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
  };

  VmaAllocationCreateInfo depthImageAllocInfo{
      .usage = VMA_MEMORY_USAGE_GPU_ONLY,
      .requiredFlags =
          VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
  };

  m_depthImage = m_application->m_allocator.createImage(&depthImageCI,
                                                        &depthImageAllocInfo);

  VkImageViewCreateInfo depthImageViewCI{
      .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext    = nullptr,
      .image    = m_depthImage.image,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format   = m_depthImageFormat,
      .subresourceRange =
          {
              .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
              .baseMipLevel   = 0,
              .levelCount     = 1,
              .baseArrayLayer = 0,
              .layerCount     = 1,
          },
  };
  vkCreateImageView(m_application->getVkDevice(), &depthImageViewCI, nullptr,
                    &m_depthImageView);
}
void Renderer::destroyDepthImages() {
  vkDestroyImageView(m_application->getVkDevice(), m_depthImageView, nullptr);
  m_application->m_allocator.destroyImage(m_depthImage);
}

void Renderer::createRenderPass(bool includeDepth, bool clear) {
  VkAttachmentDescription colorAttachment{
      .format         = m_swapchainObj->m_swapchain.image_format,
      .samples        = VK_SAMPLE_COUNT_1_BIT,
      .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  };

  VkAttachmentDescription depthAttachment{
      .format         = m_depthImageFormat,
      .samples        = VK_SAMPLE_COUNT_1_BIT,
      .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentReference colorAttachmentRef{
      .attachment = 0,
      .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentReference depthAttachmentRef{
      .attachment = 1,
      .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkSubpassDescription subpass{
      .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .colorAttachmentCount    = 1,
      .pColorAttachments       = &colorAttachmentRef,
      .pDepthStencilAttachment = nullptr,
  };
  VkSubpassDependency colorDependency{
      .srcSubpass    = VK_SUBPASS_EXTERNAL,
      .dstSubpass    = 0,
      .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                       VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0,
  };
  VkSubpassDependency depthDependency = {
      .srcSubpass   = VK_SUBPASS_EXTERNAL,
      .dstSubpass   = 0,
      .srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                      VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                      VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
  };

  VkSubpassDependency     dependencies[2]{colorDependency, depthDependency};
  VkAttachmentDescription attachments[]{colorAttachment};

  VkRenderPassCreateInfo renderPassCI{
      .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .attachmentCount = std::size(attachments),
      .pAttachments    = attachments,
      .subpassCount    = 1,
      .pSubpasses      = &subpass,
      .dependencyCount = 1,
      .pDependencies   = &colorDependency,
  };

  auto result = vkCreateRenderPass(m_application->getVkDevice(), &renderPassCI,
                                   nullptr, &m_renderPass);
  assert(result == VK_SUCCESS);
}
void Renderer::destroyRenderPass() {
  vkDestroyRenderPass(m_application->getVkDevice(), m_renderPass, nullptr);
}

void Renderer::createFrameBuffer(bool includeDepth) {
  m_frameBuffer.create(*m_application, m_swapchainObj->m_swapchain,
                       m_renderPass, {m_window.m_width, m_window.m_height},
                       m_swapchainObj->m_imageViews, std::nullopt,
                       m_graphicQueueIndex);
}

void Renderer::destroyFrameBuffer() {
  m_frameBuffer.destroy(*m_application);
}

void Renderer::createSwapchain() {
  m_graphicQueueIndex = m_application->m_deviceObj->m_device
                            .get_queue_index(vkb::QueueType::graphics)
                            .value();
  m_swapchainObj = std::make_unique<Swapchain>();
  m_swapchainObj->create(&m_application->m_deviceObj.get()->m_device, m_surface,
                         m_window.m_width, m_window.m_height);
}

void Renderer::destroySwapchain() {
  m_swapchainObj->destroy();
}

void Renderer::createShaders() {
  auto vertResult = readFromFile("shaders/main.vert.spv", "rb");
  assert(vertResult.has_value());

  Shader mainVert{"mainVert", VK_SHADER_STAGE_VERTEX_BIT};
  mainVert.create(*m_application, vertResult.value());
  m_shaders[mainVert.m_name] = std::move(mainVert);

  auto fragResult = readFromFile("shaders/main.frag.spv", "rb");
  assert(fragResult.has_value());

  Shader mainFrag{"mainFrag", VK_SHADER_STAGE_FRAGMENT_BIT};
  mainFrag.create(*m_application, fragResult.value());
  m_shaders[mainFrag.m_name] = std::move(mainFrag);
}

void Renderer::destroyShaders() {
  for (auto& [_, shader] : m_shaders) {
    shader.destroy(*m_application);
  }
}

void Renderer::createDefaultPipeline() {

  VkPipelineLayoutCreateInfo defaultPipelineLayoutCI{
      .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .pNext                  = nullptr,
      .flags                  = 0,
      .setLayoutCount         = 0,
      .pSetLayouts            = nullptr,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges    = nullptr,
  };

  vkCreatePipelineLayout(m_application->getVkDevice(), &defaultPipelineLayoutCI,
                         nullptr, &m_defaultPipelineLayout);

  m_defaultPipelineBuilder.reset(new GraphicPipelineBuilder{});
  VkDynamicState dynamicState[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                   VK_DYNAMIC_STATE_SCISSOR};
  m_defaultPipeline =
      (*m_defaultPipelineBuilder)
          .setShader({m_shaders["mainVert"].m_shaderInfo,
                      m_shaders["mainFrag"].m_shaderInfo})
          .setVertexInput(0, nullptr, 0, nullptr)
          .setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE)
          .setRasterization(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL,
                            VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE,
                            VK_FALSE, 0, 0, 0, 1)
          .setDynamic()
          .noColorBlend(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
          .setViewPortAndScissor(
              {{
                  .x        = 0.f,
                  .y        = 0.f,
                  .width    = (float)m_window.m_width,
                  .height   = (float)m_window.m_height,
                  .minDepth = 0.f,
                  .maxDepth = 1.f,
              }},
              {{
                  .offset{0, 0},
                  .extent = {m_window.m_width, m_window.m_height},
              }})
          .setDeepNoStencil(VK_TRUE, VK_COMPARE_OP_GREATER_OR_EQUAL, VK_FALSE,
                            0, 1)
          .setMultisample(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.f, nullptr,
                          VK_FALSE, VK_FALSE)
          .build(*m_application, m_renderPass, m_defaultPipelineLayout);
}

void Renderer::destroyDefaultPipeline() {
  vkDestroyPipelineLayout(m_application->getVkDevice(), m_defaultPipelineLayout,
                          nullptr);
  vkDestroyPipeline(m_application->getVkDevice(), m_defaultPipeline, nullptr);
}

void Renderer::getGraphicQueueAndQueueIndex() {
  m_graphicQueueIndex = m_application->m_deviceObj->m_device
                            .get_queue_index(vkb::QueueType::graphics)
                            .value();
  m_graphicQueue =
      m_application->m_deviceObj->m_device.get_queue(vkb::QueueType::graphics)
          .value();
}

void Renderer::createMesh() {
  VkBufferCreateInfo vertexBufferCI{
      .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext                 = nullptr,
      .flags                 = 0,
      .size                  = sizeof(g_vertices[0]) * g_vertices.size(),
      .usage                 = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      .sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 1,
      .pQueueFamilyIndices   = &m_graphicQueueIndex,
  };
  VmaAllocationCreateInfo vertexAI{
      .usage = VMA_MEMORY_USAGE_CPU_TO_GPU,
  };
  BufferAllocator& allocator = m_application->m_allocator;
  m_testMesh = allocator.createBuffer(&vertexBufferCI, &vertexAI);
  void* data;
  allocator.mmap(m_testMesh, &data);
  memcpy(data, g_vertices.data(), m_testMesh.size);
  allocator.munmap(m_testMesh);
}

void Renderer::destroyMesh() {
  BufferAllocator& allocator = m_application->m_allocator;
  allocator.destroyBuffer(m_testMesh);
}
} // namespace myvk::bs