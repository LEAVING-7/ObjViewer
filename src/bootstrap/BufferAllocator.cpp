#include "bootstrap/BufferAllocator.hpp"

namespace myvk::bs {

void Sampler::create(VkDevice device, VkFilter magFilter, VkFilter minFilter,
                     VkSamplerMipmapMode  mipmapMode,
                     VkSamplerAddressMode addressModeU,
                     VkSamplerAddressMode addressModeV,
                     VkSamplerAddressMode addressModeW, float mipLodBias,
                     VkBool32 anisotropyEnable, float maxAnisotropy,
                     VkBool32 compareEnable, VkCompareOp compareOp,
                     float minLod, float maxLod, VkBorderColor borderColor,
                     VkBool32 unnormalizedCoordinates) {
  VkSamplerCreateInfo CI{
      .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
      .pNext                   = nullptr,
      .flags                   = 0,
      .magFilter               = magFilter,
      .minFilter               = minFilter,
      .mipmapMode              = mipmapMode,
      .addressModeU            = addressModeU,
      .addressModeV            = addressModeV,
      .addressModeW            = addressModeW,
      .mipLodBias              = mipLodBias,
      .anisotropyEnable        = anisotropyEnable,
      .maxAnisotropy           = maxAnisotropy,
      .compareEnable           = compareEnable,
      .compareOp               = compareOp,
      .minLod                  = minLod,
      .maxLod                  = maxLod,
      .borderColor             = borderColor,
      .unnormalizedCoordinates = unnormalizedCoordinates,
  };
  auto result = vkCreateSampler(device, &CI, nullptr, &sampler);
  assert(result == VK_SUCCESS);
}
void Sampler::destroy(VkDevice device) {
  vkDestroySampler(device, sampler, nullptr);
}

void ImageView::create(VkDevice device, VkImage image, VkImageViewType viewType,
                       VkFormat format, VkComponentMapping components,
                       VkImageSubresourceRange range) {
  VkImageViewCreateInfo CI{
      .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext            = nullptr,
      .flags            = 0,
      .image            = image,
      .viewType         = viewType,
      .format           = format,
      .components       = components,
      .subresourceRange = range,
  };
  auto result = vkCreateImageView(device, &CI, nullptr, &imageView);
  assert(result == VK_SUCCESS);
}

void ImageView::destroy(VkDevice device) {
  vkDestroyImageView(device, imageView, nullptr);
}

void AllocatedBuffer::copyTo(AllocatedBuffer& dstBuffer, VkDevice device,
                             CommandPool pool, VkQueue transferQueue) {
  CommandBuffer stagingCmdBuffer;
  stagingCmdBuffer.alloc(device, pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

  stagingCmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  VkBufferCopy copyRegion{
      .srcOffset = 0,
      .dstOffset = 0,
      .size      = dstBuffer.size,
  };

  stagingCmdBuffer.copyBuffer(this->buffer, dstBuffer.buffer, 1, &copyRegion);
  stagingCmdBuffer.end();
  VkSubmitInfo submitInfo{
      .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers    = &stagingCmdBuffer,
  };

  vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(transferQueue);
  stagingCmdBuffer.free(device, pool);
}

void AllocatedBuffer::copyToImage(AllocatedImage&    dstImage,
                                  VkImageLayout      dstImageLayout,
                                  VkBufferImageCopy& copyRegion,
                                  VkDevice device, CommandPool pool,
                                  VkQueue transferQueue) {
  CommandBuffer stagingCmdBuffer;
  stagingCmdBuffer.alloc(device, pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  stagingCmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

  stagingCmdBuffer.copyBufferToImage(this->buffer, dstImage.image,
                                     dstImageLayout, 1, &copyRegion);

  stagingCmdBuffer.end();
  VkSubmitInfo submitInfo{
      .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers    = &stagingCmdBuffer,
  };

  vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(transferQueue);
  stagingCmdBuffer.free(device, pool);
}

void AllocatedBuffer::transferMemory(BufferAllocator& allocator,
                                     void* const data, size_t size) {
  assert(size <= this->size);
  void* mappedAddress;
  allocator.mmap(*this, &mappedAddress);
  memcpy(mappedAddress, data, size);
  allocator.munmap(*this);
}

/*
  BufferAllocator
 */
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
  ret.size        = pInfo->size;
  VkResult result = vmaCreateBuffer(m_allocator, pInfo, pAllocCreateInfo,
                                    &ret.buffer, &ret.allocation, pAllocInfo);
  assert(result == VK_SUCCESS);
  return ret;
}



void BufferAllocator::destroyBuffer(AllocatedBuffer buffer) {
  vmaDestroyBuffer(m_allocator, buffer.buffer, buffer.allocation);
}
} // namespace myvk::bs