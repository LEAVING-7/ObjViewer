#include "bootstrap/Shader.hpp"
namespace myvk::bs {

void Shader::create(VkDevice device, u32* code, size_t size, ccstr entryName) {
  assert(size % 4 == 0);
  VkResult result;

  m_shaderInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .stage = m_stage,
      .pName = entryName,
      .pSpecializationInfo = nullptr,
  };

  VkShaderModuleCreateInfo moduleCreateInfo{
      .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .pNext    = nullptr,
      .flags    = 0,
      .codeSize = size,
      .pCode    = code,
  };

  result = vkCreateShaderModule(device, &moduleCreateInfo, nullptr,
                                &m_shaderInfo.module);
  assert(result == VK_SUCCESS);
};

void Shader::destroy(VkDevice device) {
  vkDestroyShaderModule(device, m_shaderInfo.module, nullptr);
}
} // namespace myvk::bs