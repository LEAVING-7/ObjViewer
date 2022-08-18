#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "bootstrap/Command.hpp"
namespace myvk::bs {

static VkImageSubresourceRange
defaultImageSubresourceRange(VkImageAspectFlags aspectMask) {
  return {
      .aspectMask     = aspectMask,
      .baseMipLevel   = 0,
      .levelCount     = 1,
      .baseArrayLayer = 0,
      .layerCount     = 1,
  };
}

struct ImageView {
  VkImageView imageView;

public:
  void create(VkDevice device, VkImage image, VkImageViewType viewType,
              VkFormat format, VkComponentMapping components,
              VkImageSubresourceRange range);

  void destroy(VkDevice device);
};

struct Sampler {
  VkSampler sampler;

  void create(VkDevice device, VkFilter magFilter, VkFilter minFilter,
              VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
              VkSamplerAddressMode addressModeV,
              VkSamplerAddressMode addressModeW, float mipLodBias,
              VkBool32 anisotropyEnable, float maxAnisotropy,
              VkBool32 compareEnable, VkCompareOp compareOp, float minLod,
              float maxLod, VkBorderColor borderColor,
              VkBool32 unnormalizedCoordinates);
  void destroy(VkDevice device);
};

class BufferAllocator;

struct AllocatedImage {
  VkImage       image;
  VmaAllocation allocation;
  size_t        size;
};

struct AllocatedBuffer {
  VkBuffer      buffer;
  VmaAllocation allocation;
  size_t        size;

  void copyTo(AllocatedBuffer& dstBuffer, VkDevice device, CommandPool pool,
              VkQueue transferQueue);
  void copyToImage(AllocatedImage& dstImage, VkImageLayout dstImageLayout,
                   VkBufferImageCopy& copyRegion, VkDevice device,
                   CommandPool pool, VkQueue transferQueue);

  void transferMemory(BufferAllocator& allocator, void* const data,
                      size_t size);
};

class BufferAllocator {
public:
  VmaAllocator m_allocator;
  void create(VkPhysicalDevice gpu, VkDevice device, VkInstance instance);
  void destroy();

  [[nodiscard]] AllocatedImage
       createImage(VkImageCreateInfo*       pInfo,
                   VmaAllocationCreateInfo* pAllocCreateInfo,
                   VmaAllocationInfo*       pAllocInfo = nullptr);
  void destroyImage(AllocatedImage image);
  [[nodiscard]] AllocatedBuffer
       createBuffer(VkBufferCreateInfo*      pInfo,
                    VmaAllocationCreateInfo* pAllocCreateInfo,
                    VmaAllocationInfo*       pAllocInfo = nullptr);
  void destroyBuffer(AllocatedBuffer buffer);

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

  template <typename T>
  [[nodiscard]] AllocatedBuffer createBuffer(std::vector<T>&    vec,
                                             VkBufferUsageFlags bufferUsage,
                                             VmaMemoryUsage     allocUsage) {
    size_t bufferSize = sizeof(T) * vec.size();

    VkBufferCreateInfo CI{
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = nullptr,
        .flags       = 0,
        .size        = bufferSize,
        .usage       = bufferUsage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    VmaAllocationCreateInfo AI{
        .usage = allocUsage,
    };
    return createBuffer(&CI, &AI);
  }
};

} // namespace myvk::bs