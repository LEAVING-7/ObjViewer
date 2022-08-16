#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {
class Device;
class Swapchain {
public:
  vkb::Swapchain m_swapchain;

  std::vector<VkImage>     m_images;
  std::vector<VkImageView> m_imageViews;

  void create(vkb::Device* device, VkSurfaceKHR surface, u32 width, u32 height);
  void destroy();
  u32  getImageCount() {
     return m_swapchain.image_count;
  }

  MYVK_CONVERT_OP(SwapchainKHR, m_swapchain);
};
} // namespace myvk::bs