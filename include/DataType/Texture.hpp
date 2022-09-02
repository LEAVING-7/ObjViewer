#pragma once
#include "common.hpp"

#include "EasyVK/Allocator.hpp"

namespace myvk::data {
struct TextureImage {
  i32 width, height;
  i32 channels;

  ezvk::AllocatedImage image;

  void create(ezvk::Allocator& allocator, ezvk::CommandPool cmdPool,
              ccstr filename, VkQueue transferQueue, VkDevice device);
  void transitionImageLayout(VkCommandPool cmdPool, VkDevice device,
                             VkQueue transferQueue, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout);
  void destroy(ezvk::Allocator& allocator);
};

} // namespace myvk::data