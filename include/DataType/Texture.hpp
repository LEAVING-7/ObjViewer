#pragma once
#include "common.hpp"

#include "EasyVK/BufferAllocator.hpp"

namespace myvk::data {
struct TextureImage {
  i32 width, height;
  i32 channels;

   ezvk::AllocatedImage image;

  void create( ezvk::BufferAllocator& allocator,  ezvk::CommandPool cmdPool,
              ccstr filename, VkQueue transferQueue, VkDevice device);
  void transitionImageLayout(VkCommandPool cmdPool, VkDevice device,
                             VkQueue transferQueue, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout);
  void destroy( ezvk::BufferAllocator& allocator);
};

} // namespace myvk::data