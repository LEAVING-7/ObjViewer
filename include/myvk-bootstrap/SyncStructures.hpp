#include "common.hpp"
#include "pch.hpp"

namespace myvk_bs {
struct Fence {
  VkFence fence;
  void    create(VkDevice device, VkFenceCreateFlags flag);
  void    destroy(VkDevice device);
};
struct Semaphore {
  VkSemaphore semaphore;
  void        create(VkDevice device, VkSemaphoreCreateFlags flag);
  void        destroy(VkDevice device);
};
} // namespace myvk_bs