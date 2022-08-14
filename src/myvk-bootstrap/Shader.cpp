#include "myvk-bootstrap/Shader.hpp"
#include "myvk-bootstrap/Application.hpp"
namespace myvk_bs {

void Shader::create(u32 *code, size_t size, ccstr entryName) {
  assert(size % 4 == 0);

  auto    *app = Application::GetInstance();
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

  result = vkCreateShaderModule(app->getVkDevice(), &moduleCreateInfo, nullptr,
                                &m_shaderInfo.module);
  assert(result == VK_SUCCESS);
};

void Shader::destroy() {
  auto *app = Application::GetInstance();
  vkDestroyShaderModule(app->getVkDevice(), m_shaderInfo.module, nullptr);
}
} // namespace myvk_bs