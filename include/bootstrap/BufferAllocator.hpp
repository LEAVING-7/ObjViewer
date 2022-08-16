#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "bootstrap/Command.hpp"
namespace myvk::bs {

class BufferAllocator;

struct AllocatedImage {
  VkImage       image;
  VmaAllocation allocation;
};

struct AllocatedBuffer {
  VkBuffer      buffer;
  VmaAllocation allocation;
  size_t        size;

  void copyTo(AllocatedBuffer& dstBuffer, VkDevice device, CommandPool pool,
              VkQueue transferQueue);

  void transferMemory(BufferAllocator& allocator, void* const data,
                      size_t size);
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

  VkResult mmap(AllocatedBuffer& buffer, void** ppData) {
    return vmaMapMemory(m_allocator, buffer.allocation, ppData);
  }

  void munmap(AllocatedBuffer& buffer) {
    vmaUnmapMemory(m_allocator, buffer.allocation);
  }
  VkResult mmap(AllocatedImage& buffer, void** ppData) {
    return vmaMapMemory(m_allocator, buffer.allocation, ppData);
  }

  void munmap(AllocatedImage& buffer) {
    vmaUnmapMemory(m_allocator, buffer.allocation);
  }

  void flushMapedMemory(AllocatedBuffer& buffer) {
    vmaFlushAllocation(m_allocator, buffer.allocation, 0, buffer.size);
  }

  void invalidMappedMemory(AllocatedBuffer& buffer) {
    vmaInvalidateAllocation(m_allocator, buffer.allocation, 0, buffer.size);
  }
};

} // namespace myvk::bs