#include "bootstrap/Command.hpp"

namespace myvk::bs {
void CommandPool::create(VkDevice device, VkCommandPoolCreateFlags flag,
                         u32 queueIndex) {
  VkCommandPoolCreateInfo CI{
      .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .pNext            = nullptr,
      .flags            = flag,
      .queueFamilyIndex = queueIndex,
  };
  auto result = vkCreateCommandPool(device, &CI, nullptr, &cmdPool);
  assert(result == VK_SUCCESS);
}
void CommandPool::destroy(VkDevice device) {
  vkDestroyCommandPool(device, cmdPool, nullptr);
}
void CommandBuffer::create(VkDevice device, VkCommandPool cmdPool,
                           VkCommandBufferLevel level) {
  VkCommandBufferAllocateInfo AI{
      .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext              = nullptr,
      .commandPool        = cmdPool,
      .level              = level,
      .commandBufferCount = 1,
  };
  auto result = vkAllocateCommandBuffers(device, &AI, &cmdBuffer);
  assert(result == VK_SUCCESS);
}
void CommandBuffer::destroy(VkDevice device, VkCommandPool cmdPool) {
  vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
}

void CommandBuffer::reset(VkCommandBufferResetFlags flag) {
  vkResetCommandBuffer(cmdBuffer, flag);
}

void CommandBuffer::begin(
    VkCommandBufferUsageFlags             flag,
    const VkCommandBufferInheritanceInfo* pInheritanceInfo) {
  VkCommandBufferBeginInfo BI{
      .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext            = nullptr,
      .flags            = flag,
      .pInheritanceInfo = pInheritanceInfo,
  };
  auto result = vkBeginCommandBuffer(cmdBuffer, &BI);
  assert(result == VK_SUCCESS);
}
void CommandBuffer::end() {
  vkEndCommandBuffer(cmdBuffer);
}

void CommandBuffer::beginRenderPass(VkRenderPassBeginInfo* pRenderPassBI,
                                    VkSubpassContents      contents) {
  vkCmdBeginRenderPass(cmdBuffer, pRenderPassBI, contents);
}

void CommandBuffer::endRenderPass() {
  vkCmdEndRenderPass(cmdBuffer);
}
void CommandBuffer::free(VkDevice device, VkCommandPool cmdPool) {
  vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
}

} // namespace myvk_bs