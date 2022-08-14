#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk_bs {

struct AllocatedImage {
  VkImage       image;
  VmaAllocation allocation;
};

struct AllocatedBuffer {
  VkBuffer      buffer;
  VmaAllocation allocation;
};

class BufferAllocator {
public:
  VmaAllocator m_allocator;
  void create(VkPhysicalDevice gpu, VkDevice device, VkInstance instance);
  void destroy();

  AllocatedImage createImage(VkImageCreateInfo*       pInfo,
                             VmaAllocationCreateInfo* pAllocCreateInfo,
                             VmaAllocationInfo*       pAllocInfo = nullptr);
  void           destroyImage(AllocatedImage image);

  AllocatedBuffer createBuffer(VkBufferCreateInfo*      pInfo,
                               VmaAllocationCreateInfo* pAllocCreateInfo,
                               VmaAllocationInfo*       pAllocInfo = nullptr);
  void            destroyBuffer(AllocatedBuffer buffer);
};

} // namespace myvk_bs