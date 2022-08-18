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
void CommandBuffer::alloc(VkDevice device, VkCommandPool cmdPool,
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
void CommandBuffer::free(VkDevice device, VkCommandPool cmdPool) {
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

void CommandBuffer::bindVertexBuffers(u32 firstBinding, u32 bindingCount,
                                      const VkBuffer*     pBuffers,
                                      const VkDeviceSize* pOffsets) {
  vkCmdBindVertexBuffers(cmdBuffer, firstBinding, bindingCount, pBuffers,
                         pOffsets);
}

void CommandBuffer::bindVertexBuffer(VkBuffer buffer) {
  VkDeviceSize offset = 0;
  bindVertexBuffers(0, 1, &buffer, &offset);
}

void CommandBuffer::bindIndexBuffer(VkBuffer buffer, VkIndexType indexType,
                                    size_t offset) {
  vkCmdBindIndexBuffer(cmdBuffer, buffer, offset, indexType);
}

void CommandBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                               u32 regionCount, const VkBufferCopy* pRegions) {
  vkCmdCopyBuffer(cmdBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void CommandBuffer::bindPipeline(VkPipelineBindPoint bindPoint,
                                 VkPipeline          pipeline) {
  vkCmdBindPipeline(cmdBuffer, bindPoint, pipeline);
}

void CommandBuffer::bindPipelineGraphic(VkPipeline pipeline) {
  vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void CommandBuffer::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex,
                         u32 firstInstance) {
  vkCmdDraw(cmdBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandBuffer::drawIndexed(u32 indexCount, u32 instanceCount,
                                u32 firstIndex, u32 vertexOffset,
                                u32 firstInstance) {
  vkCmdDrawIndexed(cmdBuffer, indexCount, instanceCount, firstIndex,
                   vertexOffset, firstInstance);
}

void CommandBuffer::bindDescriptorSet(VkPipelineBindPoint bindPoint,
                                      VkPipelineLayout layout, u32 firstSet,
                                      u32              setCount,
                                      VkDescriptorSet* pDescriptorSets,
                                      u32              dynamicOffsetCount,
                                      u32*             pDynamicOffsets) {
  vkCmdBindDescriptorSets(cmdBuffer, bindPoint, layout, firstSet, setCount,
                          pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
void CommandBuffer::bindDescriptorSetNoDynamic(
    VkPipelineBindPoint bindPoint, VkPipelineLayout layout, u32 firstSet,
    u32 setCount, VkDescriptorSet* pDescriptorSets) {
  return bindDescriptorSet(bindPoint, layout, firstSet, setCount,
                           pDescriptorSets, 0, nullptr);
}


} // namespace myvk::bs