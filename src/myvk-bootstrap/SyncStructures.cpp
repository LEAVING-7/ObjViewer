
#include "myvk-bootstrap/SyncStructures.hpp"
namespace myvk_bs {
void Fence::create(VkDevice device, VkFenceCreateFlags flag) {
  VkFenceCreateInfo CI{
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .pNext = nullptr,
      .flags = flag,
  };
  vkCreateFence(device, &CI, nullptr, &fence);
}
void Fence::destroy(VkDevice device) { vkDestroyFence(device, fence, nullptr); }

void Semaphore::create(VkDevice device, VkSemaphoreCreateFlags flag) {
  VkSemaphoreCreateInfo CI{
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
      .pNext = nullptr,
      .flags = flag,
  };
  vkCreateSemaphore(device, &CI, nullptr, &semaphore);
}
void Semaphore::destroy(VkDevice device) {
  vkDestroySemaphore(device, semaphore, nullptr);
}

} // namespace myvk_bs