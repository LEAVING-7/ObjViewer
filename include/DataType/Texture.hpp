#pragma once
#include "common.hpp"

#include "bootstrap/BufferAllocator.hpp"


namespace myvk::data {
struct TextureImage {
  i32 width, height;
  i32 channels;

  bs::AllocatedImage image;

  void create(bs::BufferAllocator& allocator, bs::CommandPool cmdPool,
              ccstr filename, VkQueue transferQueue, VkDevice device);
  void transitionImageLayout(VkCommandPool cmdPool, VkDevice device,VkQueue transferQueue,
                             VkFormat format, VkImageLayout oldLayout,
                             VkImageLayout newLayout);
  void destroy(bs::BufferAllocator& allocator);
};

} // namespace myvk::data