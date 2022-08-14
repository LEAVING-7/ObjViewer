#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk_bs {
struct Fence {
  VkFence fence;

  void     create(VkDevice device, VkFenceCreateFlags flag);
  void     createSignaled(VkDevice device);
  void     destroy(VkDevice device);
  VkResult reset(VkDevice device);

  MYVK_CONVERT_OP(Fence, fence);
  MYVK_ADDRESS_OP(Fence, fence);
};
struct Semaphore {
  VkSemaphore semaphore;

  void create(VkDevice device);
  void destroy(VkDevice device);

  MYVK_CONVERT_OP(Semaphore, semaphore);
  MYVK_ADDRESS_OP(Semaphore, semaphore);
};
} // namespace myvk_bs