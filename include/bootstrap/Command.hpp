#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {
struct CommandPool {
  VkCommandPool cmdPool;

  void create(VkDevice device, VkCommandPoolCreateFlags flag, u32 queueIndex);
  void destroy(VkDevice device);

  MYVK_CONVERT_OP(CommandPool, cmdPool);
  MYVK_ADDRESS_OP(CommandPool, cmdPool);
};
struct CommandBuffer {
  VkCommandBuffer cmdBuffer;

  void alloc(VkDevice device, VkCommandPool cmdPool,
             VkCommandBufferLevel level);
  void free(VkDevice device, VkCommandPool cmdPool);

  void reset(VkCommandBufferResetFlags flag);
  void begin(VkCommandBufferUsageFlags             flag,
             const VkCommandBufferInheritanceInfo* pInheritanceInfo = nullptr);
  void end();
  void beginRenderPass(VkRenderPassBeginInfo* pRenderPassBI,
                       VkSubpassContents      contents);
  void endRenderPass();

  void bindVertexBuffer(VkBuffer* buffer);
  void bindVertexBuffers(u32 firstBinding, u32 bindingCount,
                         const VkBuffer*     pBuffers,
                         const VkDeviceSize* pOffsets);

  void bindIndexBuffer(VkBuffer buffer, VkIndexType indexType,
                       size_t offset = 0);
  void bindPipeline(VkPipelineBindPoint bindPoint, VkPipeline pipeline);
  void bindPipelineGraphic(VkPipeline pipeline);
  void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex,
            u32 firstInstance);
  void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex,
                   u32 vertexOffset, u32 firstInstance);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, u32 regionCount,
                  const VkBufferCopy* pRegions);

  void bindDescriptorSet(VkPipelineBindPoint bindPoint, VkPipelineLayout layout,
                         u32 firstSet, u32 setCount,
                         VkDescriptorSet* pDescriptorSets,
                         u32 dynamicOffsetCount, u32* pDynamicOffsets);
  void bindDescriptorSetNoDynamic(VkPipelineBindPoint bindPoint,
                                  VkPipelineLayout layout, u32 firstSet,
                                  u32              setCount,
                                  VkDescriptorSet* pDescriptorSets);

  MYVK_CONVERT_OP(CommandBuffer, cmdBuffer);
  MYVK_ADDRESS_OP(CommandBuffer, cmdBuffer);
};
} // namespace myvk::bs