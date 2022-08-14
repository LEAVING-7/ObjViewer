#include "myvk-bootstrap/Renderer.hpp"

#include "myvk-bootstrap/Application.hpp"
#include "myvk-bootstrap/GraphicPipelineBuilder.hpp"

namespace myvk_bs {
void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                       int mods) {
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void windowCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {}

Renderer::Renderer(Application* app) : m_application(app) {}
Renderer::~Renderer() {}

void Renderer::create() {
  createDepthImages();
  m_swapchainObj =
      std::make_unique<Swapchain>(&m_application->m_deviceObj->m_device);
  m_swapchainObj->create(m_windowExtent.width, m_windowExtent.height);
  createRenderPass(true);
  createFrameBuffer(true);
  createShaders();
  createDefaultPipeline();
}

void Renderer::destroy() {
  destroyDefaultPipeline();
  destroyShaders();
  destroyFrameBuffer();
  destroyRenderPass();
  m_swapchainObj->destroy();
  destroyDepthImages();
}

void Renderer::prepare() {}
void Renderer::render() { glfwPollEvents(); }
bool Renderer::windowShouldClose() { return glfwWindowShouldClose(m_window); }
void Renderer::createWindow(u32 width, u32 height) {
  m_windowExtent = {width, height};
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwSetErrorCallback([](int error_code, const char* description) {
    LOG_ERR("[glfw] [Error code: {}]: {}", error_code, description);
  });
  m_window = glfwCreateWindow(width, height, "Vulkan Title", nullptr, nullptr);
  glfwCreateWindowSurface(m_application->getVkInstance(), m_window, nullptr,
                          &m_surface);
  glfwSetKeyCallback(m_window, windowKeyCallback);
  glfwSetCursorPosCallback(m_window, windowCursorPosCallback);
}

void Renderer::destroyWindow() {
  vkDestroySurfaceKHR(m_application->getVkInstance(), m_surface, nullptr);
  glfwDestroyWindow(m_window);
}

void Renderer::createDepthImages() {
  VkExtent3D imageExtent{
      .width  = m_windowExtent.width,
      .height = m_windowExtent.height,
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

  VkAttachmentReference colorAttachmentRef{
      .attachment = 0,
      .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentDescription depthAttachment{
      .format         = m_depthImageFormat,
      .samples        = VK_SAMPLE_COUNT_1_BIT,
      .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentReference depthAttachmentRef{
      .attachment = 1,
      .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkSubpassDescription subpass{
      .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .colorAttachmentCount    = 1,
      .pColorAttachments       = &colorAttachmentRef,
      .pDepthStencilAttachment = &depthAttachmentRef,
  };
  VkSubpassDependency colorDependency{
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask   = 0,
      .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
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
  VkAttachmentDescription attachments[2]{colorAttachment, depthAttachment};

  VkRenderPassCreateInfo renderPassCI{
      .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .attachmentCount = 2,
      .pAttachments    = &attachments[0],
      .subpassCount    = 1,
      .pSubpasses      = &subpass,
      .dependencyCount = 0,
      .pDependencies   = nullptr,
  };

  auto result = vkCreateRenderPass(m_application->getVkDevice(), &renderPassCI,
                                   nullptr, &m_renderPass);
  assert(result == VK_SUCCESS);
}

void Renderer::destroyRenderPass() {
  vkDestroyRenderPass(m_application->getVkDevice(), m_renderPass, nullptr);
}

void Renderer::createFrameBuffer(bool includeDepth) {
  VkFramebufferCreateInfo fbCI{
      .sType      = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .pNext      = nullptr,
      .flags      = 0,
      .renderPass = m_renderPass,
      .width      = m_windowExtent.width,
      .height     = m_windowExtent.height,
      .layers     = 1,
  };

  u32 swapchainSize = u32(m_swapchainObj->m_images.size());
  LOG_INFO("swapchain size:{}", swapchainSize);
  m_framebuffers.resize(swapchainSize);

  for (u32 i = 0; i < swapchainSize; ++i) {
    VkImageView attachments[2]{m_swapchainObj->m_imageViews[i],
                               m_depthImageView};
    fbCI.attachmentCount = 2;
    fbCI.pAttachments    = attachments;
    auto result = vkCreateFramebuffer(m_application->getVkDevice(), &fbCI,
                                      nullptr, &m_framebuffers[i]);
    assert(result == VK_SUCCESS);
  }
}
void Renderer::destroyFrameBuffer() {
  for (auto& framebuffer : m_framebuffers) {
    vkDestroyFramebuffer(m_application->getVkDevice(), framebuffer, nullptr);
  }
}

void Renderer::createShaders() {
  auto vertResult = readFromFile("shaders/main.vert.spv", "rb");
  assert(vertResult.has_value());

  Shader mainVert{"mainVert", VK_SHADER_STAGE_VERTEX_BIT};
  mainVert.create(vertResult.value());
  m_shaders[mainVert.m_name] = std::move(mainVert);

  auto fragResult = readFromFile("shaders/main.frag.spv", "rb");
  assert(fragResult.has_value());

  Shader mainFrag{"mainFrag", VK_SHADER_STAGE_FRAGMENT_BIT};
  mainFrag.create(fragResult.value());
  m_shaders[mainFrag.m_name] = std::move(mainFrag);
}

void Renderer::destroyShaders() {
  for (auto& [_, shader] : m_shaders) {
    shader.destroy();
  }
}

void Renderer::createDefaultPipeline() {

  VkPipelineLayoutCreateInfo defaultPipelineLayoutCI{
      .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .pNext                  = nullptr,
      .flags                  = 0,
      .setLayoutCount         = 0,
      .pushConstantRangeCount = 0,
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
          .setDynamic(u32(std::size(dynamicState)), dynamicState)
          .noColorBlend(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
          .setViewPortAndScissor({{
                                     .x        = 0.f,
                                     .y        = 0.f,
                                     .width    = (float)m_windowExtent.width,
                                     .height   = (float)m_windowExtent.height,
                                     .minDepth = 0.f,
                                     .maxDepth = 1.f,
                                 }},
                                 {{
                                     .offset{0, 0},
                                     .extent = m_windowExtent,
                                 }})
          .setDeepNoStencil(VK_TRUE, VK_COMPARE_OP_GREATER_OR_EQUAL, VK_FALSE,
                            0, 1)
          .setMultisample(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.f, nullptr,
                          VK_FALSE, VK_FALSE)
          .build(m_application->getVkDevice(), m_renderPass,
                 m_defaultPipelineLayout);
}
void Renderer::destroyDefaultPipeline() {
  vkDestroyPipelineLayout(m_application->getVkDevice(), m_defaultPipelineLayout,
                          nullptr);
  vkDestroyPipeline(m_application->getVkDevice(), m_defaultPipeline, nullptr);
}

void Renderer::createCommand() {
  VkCommandPoolCreateInfo commandPoolCI{
      .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .pNext            = nullptr,
      .flags            = 0,
      .queueFamilyIndex = m_application->m_deviceObj->m_device
                              .get_queue_index(vkb::QueueType::graphics)
                              .value(),
  };
  auto result =
      vkCreateCommandPool(m_application->getVkDevice(), &commandPoolCI, nullptr,
                          &m_mainCommandPool);
  assert(result == VK_SUCCESS);

  VkCommandBufferAllocateInfo cmdAI{
      .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext              = nullptr,
      .commandPool        = m_mainCommandPool,
      .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1,
  };
  vkAllocateCommandBuffers(m_application->getVkDevice(), &cmdAI, &m_mainCommandBuffer);

}
void Renderer::destroyCommand() {
  vkDestroyCommandPool(m_application->getVkDevice(), m_mainCommandPool,
                       nullptr);
}
} // namespace myvk_bs