#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {

struct DescriptorPoolSizeList {
  std::vector<VkDescriptorPoolSize> list;

  DescriptorPoolSizeList& add(VkDescriptorType type, u32 descriptorCount) {
    list.push_back({type, descriptorCount});
    return *this;
  }
};
struct DescriptorPool {
  VkDescriptorPool descPool;

  VkResult create(VkDevice device, VkDescriptorPoolCreateFlags flags,
                  u32 maxSets, u32 poolSizeCount,
                  VkDescriptorPoolSize* pPoolSize);

  VkResult create(VkDevice device, VkDescriptorPoolCreateFlags flags,
                  u32 maxSets, std::vector<VkDescriptorPoolSize>& poolSizes) {
    return create(device, flags, maxSets, u32(poolSizes.size()),
                  poolSizes.data());
  }

  void destroy(VkDevice device);

  [[nodiscard]] VkDescriptorSet allocSet(VkDevice              device,
                                         VkDescriptorSetLayout setLayout);
  void                          freeSet(VkDevice device, VkDescriptorSet set);

  [[nodiscard]] std::vector<VkDescriptorSet>
  allocSets(VkDevice device, u32 setCount, VkDescriptorSetLayout* pSetLayout);
  [[nodiscard]] std::vector<VkDescriptorSet>
  allocSets(VkDevice device, std::vector<VkDescriptorSetLayout>& setLayouts) {
    return allocSets(device, u32(setLayouts.size()), setLayouts.data());
  }

  void freeSets(VkDevice device, u32 setCount,
                VkDescriptorSetLayout* pSetLayout);
  void freeSets(VkDevice device, std::vector<VkDescriptorSet>& sets);
};
struct DescriptorSetLayoutBindingList {
  std::vector<VkDescriptorSetLayoutBinding> bindings;

  DescriptorSetLayoutBindingList&
  add(u32 binding, VkDescriptorType descriptorType, u32 descriptorCount,
      VkShaderStageFlags stageFlags,
      const VkSampler*   pImmutableSampler = nullptr) {
    bindings.push_back({binding, descriptorType, descriptorCount, stageFlags,
                        pImmutableSampler});
    return *this;
  }
};

struct DescriptorSetLayout {
  VkDescriptorSetLayout setLayout;

  VkResult create(VkDevice device, u32 bindingCount,
                  VkDescriptorSetLayoutBinding* pBindings);

  VkResult create(VkDevice                                   device,
                  std::vector<VkDescriptorSetLayoutBinding>& bindings) {
    return create(device, u32(bindings.size()), bindings.data());
  }

  void destroy(VkDevice device);
};

} // namespace myvk::bs