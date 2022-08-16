#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {
struct DescriptorPool {
  VkDescriptorPool descPool;

  VkResult create(VkDevice device, u32 maxSets, u32 poolSizeCount,
                  VkDescriptorPoolSize* pPoolSize);
  void     destroy(VkDevice device);

  VkDescriptorSet allocSet(VkDevice device, VkDescriptorSetLayout setLayout);
  void            freeSet(VkDevice device, VkDescriptorSet set);

  std::vector<VkDescriptorSet> allocSets(VkDevice device, u32 setCount,
                                         VkDescriptorSetLayout* pSetLayout);
  void freeSets(VkDevice device, std::vector<VkDescriptorSet>& sets);
};

struct DescriptorSet {
  VkDescriptorSet set;
};

struct DescriptorSetLayout {
  VkDescriptorSetLayout setLayout;

  VkResult create(VkDevice device, u32 bindingCount,
                  VkDescriptorSetLayoutBinding* pBindings);

  void destroy(VkDevice device);
};

} // namespace myvk::bs