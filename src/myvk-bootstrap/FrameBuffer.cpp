#include "myvk-bootstrap/FrameBuffer.hpp"
#include "myvk-bootstrap/Swapchain.hpp"

namespace myvk_bs {

void Framebuffer::FrameData::create(VkDevice device, u32 graphicIndex) {
  create1(device, presentSemaphore, renderSemaphore);
  // create signaled fence
  renderFence.createSignaled(device);
  cmdPool.create(device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                 graphicIndex);
  cmdBuffer.create(device, cmdPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
}

void Framebuffer::FrameData::destroy(VkDevice device) {
  cmdBuffer.destroy(device, cmdPool);
  destroy1(device, presentSemaphore, renderSemaphore, renderFence);
  cmdPool.destroy(device);
}

void Framebuffer::create(VkDevice device, vkb::Swapchain& swapchain,
                         VkRenderPass renderPass, VkExtent2D windowExtent,
                         std::vector<VkImageView>& imageViews,
                         VkImageView depthImage, u32 graphicIndex) {
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
    VkImageView attachments[2]{imageViews[i], depthImage};
    CI.attachmentCount = 2;
    CI.pAttachments    = attachments;
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

u32 Framebuffer::acquireNextImage(VkDevice device, VkSwapchainKHR swapchain,
                                  u64 timeout) {
  u32 imageIdx;
  vkAcquireNextImageKHR(device, swapchain, 100000000,
                        currentFrameData().presentSemaphore, nullptr,
                        &imageIdx);
  return imageIdx;
}
} // namespace myvk_bs