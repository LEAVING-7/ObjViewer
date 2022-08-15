#include "bootstrap/BufferAllocator.hpp"

namespace myvk::bs {
void BufferAllocator::create(VkPhysicalDevice gpu, VkDevice device,
                             VkInstance instance) {
  VmaAllocatorCreateInfo allocatorCI{
      .physicalDevice = gpu,
      .device         = device,
      .instance       = instance,
  };
  vmaCreateAllocator(&allocatorCI, &m_allocator);
}
void BufferAllocator::destroy() {
  vmaDestroyAllocator(m_allocator);
}

AllocatedImage
BufferAllocator::createImage(VkImageCreateInfo*       pInfo,
                             VmaAllocationCreateInfo* pAllocCreateInfo,
                             VmaAllocationInfo*       pAllocInfo) {
  AllocatedImage ret;
  VkResult       result = vmaCreateImage(m_allocator, pInfo, pAllocCreateInfo,
                                         &ret.image, &ret.allocation, pAllocInfo);
  assert(result == VK_SUCCESS);
  return ret;
}
void BufferAllocator::destroyImage(AllocatedImage image) {
  vmaDestroyImage(m_allocator, image.image, image.allocation);
}
AllocatedBuffer
BufferAllocator::createBuffer(VkBufferCreateInfo*      pInfo,
                              VmaAllocationCreateInfo* pAllocCreateInfo,
                              VmaAllocationInfo*       pAllocInfo) {
  AllocatedBuffer ret;
  ret.size = pInfo->size;
  VkResult result = vmaCreateBuffer(m_allocator, pInfo, pAllocCreateInfo,
                                    &ret.buffer, &ret.allocation, pAllocInfo);
  assert(result == VK_SUCCESS);
  return ret;
}
void BufferAllocator::destroyBuffer(AllocatedBuffer buffer) {
  vmaDestroyBuffer(m_allocator, buffer.buffer, buffer.allocation);
}
} // namespace myvk::bs