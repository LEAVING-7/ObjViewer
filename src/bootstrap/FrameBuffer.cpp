#include "bootstrap/FrameBuffer.hpp"
#include "bootstrap/Swapchain.hpp"

namespace myvk::bs {
void Framebuffer::FrameData::create(VkDevice device, u32 graphicIndex) {
  create1(device, presentSemaphore, renderSemaphore);
  // create signaled fence
  renderFence.createSignaled(device);
  mainCmdPool.create(device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                     graphicIndex);
  cmdBuffer.alloc(device, mainCmdPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
}

void Framebuffer::FrameData::destroy(VkDevice device) {
  cmdBuffer.free(device, mainCmdPool);
  mainCmdPool.destroy(device);
  destroy1(device, presentSemaphore, renderSemaphore, renderFence);
}

void Framebuffer::create(VkDevice device, vkb::Swapchain& swapchain,
                         VkRenderPass renderPass, VkExtent2D windowExtent,
                         std::vector<VkImageView>&  imageViews,
                         std::optional<VkImageView> depthImageView,
                         u32                        graphicIndex, u32 transferIndex) {

  for (auto& frame : frameData) {
    frame.create(device, graphicIndex);
  }

  VkFramebufferCreateInfo CI{
      .sType      = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .pNext      = nullptr,
      .flags      = 0,
      .renderPass = renderPass,
      .width      = windowExtent.width,
      .height     = windowExtent.height,
      .layers     = 1,
  };

  u32 swapchainSize = u32(swapchain.image_count);
  framebuffers.resize(swapchainSize);
  for (u32 i = 0; i < swapchainSize; ++i) {
    if (!depthImageView.has_value()) {
      CI.attachmentCount = 1;
      CI.pAttachments    = &imageViews[i];
    } else {
      VkImageView attachments[]{imageViews[i], depthImageView.value()};
      CI.attachmentCount = 2;
      CI.pAttachments    = attachments;
    }
    auto result = vkCreateFramebuffer(device, &CI, nullptr, &framebuffers[i]);
    assert(result == VK_SUCCESS);
  }
}
void Framebuffer::destroy(VkDevice device) {
  for (auto& frame : frameData) {
    frame.destroy(device);
  }

  for (auto& frame : framebuffers) {
    vkDestroyFramebuffer(device, frame, nullptr);
  }
}

VkResult Framebuffer::acquireNextImage(VkDevice       device,
                                       VkSwapchainKHR swapchain, u64 timeout,
                                       u32* index) {
  auto result = vkAcquireNextImageKHR(device, swapchain, 100000000,
                                      currentFrameData().presentSemaphore,
                                      nullptr, index);
  return result;
}

void Framebuffer::freeCmdBuffer(VkDevice device) {
  for (FrameData& frame : frameData) {
    frame.cmdBuffer.free(device, frameData->mainCmdPool);
  }
}
} // namespace myvk::bs