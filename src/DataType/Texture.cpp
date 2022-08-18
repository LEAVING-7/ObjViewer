#include "DataType/Texture.hpp"


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace myvk::data {

void TextureImage::create(bs::BufferAllocator& allocator,
                          bs::CommandPool cmdPool, ccstr filename,
                          VkQueue transferQueue, VkDevice device) {
  stbi_uc* pixel =
      stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
  size_t imageSize = width * height * 4;
  if (!pixel) {
    LOG_ERR("Load image {} failed", filename);
    exit(-1);
  }

  VkBufferCreateInfo stagingBufferCI{
      .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext       = nullptr,
      .flags       = 0,
      .size        = imageSize,
      .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  VmaAllocationCreateInfo stagingBufferAI{
      .usage = VMA_MEMORY_USAGE_CPU_ONLY,
  };
  bs::AllocatedBuffer stagingBuffer =
      allocator.createBuffer(&stagingBufferCI, &stagingBufferAI);

  stagingBuffer.transferMemory(allocator, (void*)(pixel), stagingBuffer.size);

  VkImageCreateInfo imageCI{
      .sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext       = nullptr,
      .flags       = 0,
      .imageType   = VK_IMAGE_TYPE_2D,
      .format      = VK_FORMAT_R8G8B8A8_SRGB,
      .extent      = {(u32)width, (u32)height, 1},
      .mipLevels   = 1,
      .arrayLayers = 1,
      .samples     = VK_SAMPLE_COUNT_1_BIT,
      .tiling      = VK_IMAGE_TILING_OPTIMAL,
      .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
  };

  VmaAllocationCreateInfo vmaCI{
      .usage = VMA_MEMORY_USAGE_GPU_ONLY,
  };
  this->image = allocator.createImage(&imageCI, &vmaCI);

  transitionImageLayout(cmdPool, device, transferQueue, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  VkBufferImageCopy copyRegion{
      .bufferOffset      = 0,
      .bufferRowLength   = 0,
      .bufferImageHeight = 0,
      .imageSubresource =
          {
              .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
              .mipLevel       = 0,
              .baseArrayLayer = 0,
              .layerCount     = 1,
          },
      .imageOffset = {0, 0, 0},
      .imageExtent = {(u32)width, (u32)height, 1},
  };
  stagingBuffer.copyToImage(this->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            copyRegion, device, cmdPool, transferQueue);

  transitionImageLayout(cmdPool, device, transferQueue, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  allocator.destroyBuffer(stagingBuffer);
  stbi_image_free(pixel);
}

void TextureImage::destroy(bs::BufferAllocator& allocator) {
  allocator.destroyImage(image);
}

void TextureImage::transitionImageLayout(VkCommandPool cmdPool, VkDevice device,
                                         VkQueue transferQueue, VkFormat format,
                                         VkImageLayout oldLayout,
                                         VkImageLayout newLayout) {
  // image transition
  bs::CommandBuffer cmd;
  cmd.alloc(device, cmdPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  cmd.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

  VkImageMemoryBarrier barrier{
      .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .oldLayout           = oldLayout,
      .newLayout           = newLayout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image               = this->image.image,
      .subresourceRange =
          {
              .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel   = 0,
              .levelCount     = 1,
              .baseArrayLayer = 0,
              .layerCount     = 1,
          },
  };

  VkPipelineStageFlags srcStage, dstStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    LOG_ERR("invalid argument");
    exit(-1);
  }

  cmd.pipelineImageBarriers(srcStage, dstStage, 0, 1, &barrier);
  cmd.end();

  VkSubmitInfo submitInfo{
      .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .pNext              = nullptr,
      .commandBufferCount = 1,
      .pCommandBuffers    = &cmd.cmdBuffer,
  };
  vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);

  vkDeviceWaitIdle(device);
  cmd.free(device, cmdPool);
}

} // namespace myvk::data