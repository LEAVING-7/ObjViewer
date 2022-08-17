#include "Application/Renderer.hpp"
#include "Application/Application.hpp"

#include "DataType/Vertex.hpp"

namespace myvk::bs {

float g_frameBegin = 0;
float g_frameEnd   = 0;

const std::vector<data::Vertex> g_vertices = {
    {{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f}},
};

const std::vector<u16> g_indices = {
    0, 1, 2, 2, 3, 0,
};

const std::vector<data::Vertex> g_axis = {
    {{0, 0, 0}, {0, 0, 0}}, // original
    {{1, 0, 0}, {1, 0, 0}}, // x
    {{0, 1, 0}, {0, 1, 0}}, // y
    {{0, 0, 1}, {0, 0, 1}}, // z
};

const std::vector<u16> g_axisIndices = {
    0, 1, 0, 2, 0, 3,
};

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
} g_uniformData;

void windowCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  Renderer* renderer = gui::MainWindow::getUserPointer<Renderer*>(window);
  if (renderer->m_window.getKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
    renderer->m_window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return;
  } else {
    renderer->m_window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }
  float windowWidth  = renderer->m_window.m_width / 2;
  float windowHeight = renderer->m_window.m_height / 2;
  xpos -= windowWidth;
  ypos -= windowHeight;
  renderer->m_state.camera.processMouseMovement(xpos, ypos);
  glfwSetCursorPos(window, windowWidth, windowHeight);
}

void windowFramebufferResizeCallback(GLFWwindow* window, int width,
                                     int height) {
  Renderer* renderer = gui::MainWindow::getUserPointer<Renderer*>(window);
  renderer->m_window.m_framebufferResized = true;
}

void Renderer::create(Application* app) {
  m_application = app;
  getGraphicQueueAndQueueIndex();
  // createDepthImages();
  createSwapchain();
  createRenderPass(true);
  createShaders();
  createDescriptorSets();
  createDefaultPipeline();
  createFrameBuffer(true);

  createMesh();
}

void Renderer::destroy() {
  vkDeviceWaitIdle(*m_application);

  destroyMesh();

  destroyFrameBuffer();
  destroyDefaultPipeline();
  destroyDescriptorSets();
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
  // destroyDepthImages();

  m_window.updateExtent();

  // createDepthImages();
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

  if (m_window.getKey(GLFW_KEY_W) == GLFW_PRESS) {
    m_state.camera.move(data::Camera::MoveDirection::eForward);
  }
  if (m_window.getKey(GLFW_KEY_A) == GLFW_PRESS) {
    m_state.camera.move(data::Camera::MoveDirection::eLeft);
  }
  if (m_window.getKey(GLFW_KEY_S) == GLFW_PRESS) {
    m_state.camera.move(data::Camera::MoveDirection::eBackward);
  }
  if (m_window.getKey(GLFW_KEY_D) == GLFW_PRESS) {
    m_state.camera.move(data::Camera::MoveDirection::eRight);
  }
  if (m_window.getKey(GLFW_KEY_Q) == GLFW_PRESS) {
    m_state.camera.move(data::Camera::MoveDirection::eUp);
  }
  if (m_window.getKey(GLFW_KEY_E) == GLFW_PRESS) {
    m_state.camera.move(data::Camera::MoveDirection::eDown);
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

  // automatically set cmdBuffer to initial
  currentData.cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

  currentData.cmdBuffer.beginRenderPass(&renderPassBI,
                                        VK_SUBPASS_CONTENTS_INLINE);
  currentData.cmdBuffer.bindPipelineGraphic(m_defaultPipeline);

  float time = glfwGetTime();

  g_uniformData.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f));

  g_uniformData.view = m_state.camera.viewMat();
  g_uniformData.proj = glm::perspective(
      glm::radians(45.f), m_window.m_width / (float)m_window.m_height, 0.1f,
      100.0f);

  g_uniformData.proj[1][1] *= -1;

  m_uniformBuffer.transferMemory(m_application->m_allocator, &g_uniformData,
                                 sizeof(g_uniformData));

  currentData.cmdBuffer.bindVertexBuffer(&m_testMesh.buffer);
  currentData.cmdBuffer.bindIndexBuffer(m_testIndex.buffer,
                                        VK_INDEX_TYPE_UINT16);
  currentData.cmdBuffer.bindDescriptorSetNoDynamic(
      VK_PIPELINE_BIND_POINT_GRAPHICS, m_defaultPipelineLayout, 0, 1,
      &m_uniformSets[swapchainImgIdx]);

  // currentData.cmdBuffer.draw(g_vertices.size(), 1, 0, 0);
  currentData.cmdBuffer.drawIndexed(g_indices.size(), 1, 0, 0, 0);
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
      .setFramebufferSizeCallback(windowFramebufferResizeCallback)
      .setKeyCallback(
          [](GLFWwindow* wnd, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE) {
              glfwSetWindowShouldClose(wnd, GLFW_TRUE);
            }
          })
      .setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

  auto result =
      vkCreateRenderPass(*m_application, &renderPassCI, nullptr, &m_renderPass);
  assert(result == VK_SUCCESS);
}

void Renderer::destroyRenderPass() {
  vkDestroyRenderPass(*m_application, m_renderPass, nullptr);
}

void Renderer::createFrameBuffer(bool includeDepth) {
  m_frameBuffer.create(*m_application, m_swapchainObj->m_swapchain,
                       m_renderPass, {m_window.m_width, m_window.m_height},
                       m_swapchainObj->m_imageViews, std::nullopt,
                       m_graphicQueueIndex, m_transferQueueIndex);
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
      .setLayoutCount         = 1,
      .pSetLayouts            = &m_uniformLayout.setLayout,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges    = nullptr,
  };

  vkCreatePipelineLayout(m_application->getVkDevice(), &defaultPipelineLayoutCI,
                         nullptr, &m_defaultPipelineLayout);

  m_defaultPipelineBuilder.reset(new GraphicPipelineBuilder{});
  VkDynamicState dynamicState[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                   VK_DYNAMIC_STATE_SCISSOR};
  auto [defaultPipeline, defaultPipelineCache] =
      (*m_defaultPipelineBuilder)
          .setShader({m_shaders["mainVert"].m_shaderInfo,
                      m_shaders["mainFrag"].m_shaderInfo})
          .setVertexInput(data::Vertex::GetDescription())
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
          .buildWithCache(*m_application, m_renderPass,
                          m_defaultPipelineLayout);
  m_defaultPipeline      = defaultPipeline;
  m_defaultPipelineCache = defaultPipelineCache;


  
}

void Renderer::destroyDefaultPipeline() {
  vkDestroyPipelineLayout(*m_application, m_defaultPipelineLayout, nullptr);
  vkDestroyPipelineCache(*m_application, m_defaultPipelineCache, nullptr);
  vkDestroyPipeline(*m_application, m_defaultPipeline, nullptr);
}

void Renderer::getGraphicQueueAndQueueIndex() {

  vkb::Device& device = m_application->m_deviceObj->m_device;
  m_graphicQueueIndex =
      device.get_queue_index(vkb::QueueType::graphics).value();
  m_graphicQueue = device.get_queue(vkb::QueueType::graphics).value();

  m_transferQueueIndex =
      device.get_queue_index(vkb::QueueType::transfer).value();
  m_transferQueue = device.get_queue(vkb::QueueType::transfer).value();
}

void Renderer::createMesh() {

  auto& currentFrame = m_frameBuffer.currentFrameData();

  VkBufferCreateInfo vertexBufferCI{
      .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext       = nullptr,
      .flags       = 0,
      .size        = sizeof(g_vertices[0]) * g_vertices.size(),
      .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };
  VmaAllocationCreateInfo vertexAI{
      .usage = VMA_MEMORY_USAGE_CPU_ONLY,
  };

  BufferAllocator& allocator = m_application->m_allocator;
  AllocatedBuffer  stagingBuffer =
      allocator.createBuffer(&vertexBufferCI, &vertexAI);
  stagingBuffer.transferMemory(allocator, (void*)(g_vertices.data()),
                               stagingBuffer.size);
  VkBufferCreateInfo dstBufferCI{
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .size  = stagingBuffer.size,
      .usage =
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };
  vertexAI.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  m_testMesh     = allocator.createBuffer(&dstBufferCI, &vertexAI);

  stagingBuffer.copyTo(m_testMesh, *m_application, currentFrame.stagingCmdPool,
                       m_transferQueue);
  allocator.destroyBuffer(stagingBuffer);

  VkBufferCreateInfo indexCI{
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .size  = sizeof(g_indices[0]) * g_indices.size(),
      .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
  };
  VmaAllocationCreateInfo indexAI{
      .usage = VMA_MEMORY_USAGE_CPU_TO_GPU,
  };
  m_testIndex = allocator.createBuffer(&indexCI, &indexAI);
  m_testIndex.transferMemory(allocator, (void*)g_indices.data(),
                             m_testIndex.size);
}

void Renderer::destroyMesh() {
  BufferAllocator& allocator = m_application->m_allocator;
  allocator.destroyBuffer(m_testMesh);
  allocator.destroyBuffer(m_testIndex);
}

void Renderer::createDescriptorSets() {
  auto& allocator = m_application->m_allocator;

  DescriptorPoolSizeList sizeList;
  sizeList.add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
               m_swapchainObj->getImageCount());

  m_descPool.create(*m_application,
                    VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, 8,
                    sizeList.list);

  DescriptorSetLayoutBindingList bindingList;
  bindingList.add(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
                  VK_SHADER_STAGE_VERTEX_BIT);
  m_uniformLayout.create(*m_application, bindingList.bindings);

  std::vector<VkDescriptorSetLayout> layouts(m_swapchainObj->getImageCount(),
                                             m_uniformLayout.setLayout);
  m_uniformSets = m_descPool.allocSets(*m_application, layouts);

  VkBufferCreateInfo uniformBufferCI{
      .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext       = nullptr,
      .flags       = 0,
      .size        = sizeof(g_uniformData),
      .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };
  VmaAllocationCreateInfo uniformBufferAI{.usage = VMA_MEMORY_USAGE_CPU_TO_GPU};

  m_uniformBuffer = allocator.createBuffer(&uniformBufferCI, &uniformBufferAI);

  VkDescriptorBufferInfo uniformBufferInfo{
      .buffer = m_uniformBuffer.buffer,
      .offset = 0,
      .range  = VK_WHOLE_SIZE,
  };

  for (u32 i = 0; i < m_uniformSets.size(); ++i) {
    VkWriteDescriptorSet writeSet{
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext           = nullptr,
        .dstSet          = m_uniformSets[i],
        .dstBinding      = bindingList.bindings[0].binding,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .pBufferInfo     = &uniformBufferInfo,
    };
    vkUpdateDescriptorSets(*m_application, 1, &writeSet, 0, nullptr);
  }
}
void Renderer::destroyDescriptorSets() {
  m_application->m_allocator.destroyBuffer(m_uniformBuffer);

  m_uniformLayout.destroy(*m_application);
  m_descPool.freeSets(*m_application, m_uniformSets);
  m_descPool.destroy(*m_application);
}
} // namespace myvk::bs