#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "bootstrap/Command.hpp"
#include "bootstrap/SyncStructures.hpp"

// TODO : FrameBuffer
namespace myvk::bs {
class Framebuffer {
public:
  struct FrameData {
    Semaphore     presentSemaphore, renderSemaphore;
    Fence         renderFence;
    CommandPool   cmdPool;
    CommandBuffer cmdBuffer;

    void create(VkDevice device, u32 graphicIndex);
    void destroy(VkDevice device);
  };

  static constexpr u32 DATA_COUNT = 2;

  u64                        frameCount{0};
  std::vector<VkFramebuffer> framebuffers;
  FrameData                  frameData[DATA_COUNT];

  void create(VkDevice device, vkb::Swapchain& swapchain,
              VkRenderPass renderPass, VkExtent2D windowExtent,
              std::vector<VkImageView>& imageViews, VkImageView depthImage,
              u32 graphicIndex);
  void destroy(VkDevice device);

  // using present semaphore
  VkResult acquireNextImage(VkDevice device, VkSwapchainKHR swapchain, u64 timeout, u32 *index);
  void freeCmdBuffer(VkDevice device);

  void updateFrameCount() {
    ++frameCount;
  }
  FrameData& currentFrameData() {
    return frameData[frameCount % DATA_COUNT];
  }
};
} // namespace myvk_bs