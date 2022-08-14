#pragma once

#include "common.hpp"
#include "pch.hpp"

namespace myvk_bs {
class Swapchain {
public:
  vkb::Swapchain m_swapchain;
  vkb::Device*   m_device;

  std::vector<VkImage>     m_images;
  std::vector<VkImageView> m_imageViews;

  Swapchain(vkb::Device* device) : m_device(device) {}
  ~Swapchain();

  void create(u32 width, u32 height);
  void destroy();

  MYVK_CONVERT_OP(SwapchainKHR, m_swapchain);
};
} // namespace myvk_bs