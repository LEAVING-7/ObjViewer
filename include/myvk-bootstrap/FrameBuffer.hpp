#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "myvk-bootstrap/Command.hpp"
#include "myvk-bootstrap/SyncStructures.hpp"

// TODO : FrameBuffer
namespace myvk_bs {
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
  u32 acquireNextImage(VkDevice device, VkSwapchainKHR swapchain, u64 timeout);

  inline void       updateFrameCount();
  inline FrameData& currentFrameData();
};
} // namespace myvk_bs