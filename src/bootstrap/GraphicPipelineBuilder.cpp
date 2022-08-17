#include "bootstrap/GraphicPipelineBuilder.hpp"

namespace myvk::bs {
GraphicPipelineBuilder& GraphicPipelineBuilder::setShader(
    std::vector<VkPipelineShaderStageCreateInfo>&& shaders) {
  this->shaders = std::forward<decltype(shaders)>(shaders);
  return *this;
}

GraphicPipelineBuilder&
GraphicPipelineBuilder::noColorBlend(VkColorComponentFlags colorWriteMask) {
  this->colorBlendAttachment = VkPipelineColorBlendAttachmentState{
      .blendEnable    = VK_FALSE,
      .colorWriteMask = colorWriteMask,
  };
  return *this;
}

GraphicPipelineBuilder&
GraphicPipelineBuilder::setDynamic(uint32_t              dynamicStateCount,
                                   const VkDynamicState* pDynamicStates) {
  this->dynamic = {
      .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .pNext             = nullptr,
      .flags             = 0,
      .dynamicStateCount = dynamicStateCount,
      .pDynamicStates    = pDynamicStates,
  };
  return *this;
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setVertexInput(
    data::VertexInputDescription&& vertDesc) {
  return setVertexInput(vertDesc.bindings.size(), vertDesc.bindings.data(),
                        vertDesc.attributes.size(), vertDesc.attributes.data());
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setVertexInput(
    u32                                      vertexBindingDescriptionCount,
    const VkVertexInputBindingDescription*   pVertexBindingDescriptions,
    u32                                      vertexAttributeDescriptionCount,
    const VkVertexInputAttributeDescription* pVertexAttributeDescriptions) {
  this->vertexInputInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .vertexBindingDescriptionCount   = vertexBindingDescriptionCount,
      .pVertexBindingDescriptions      = pVertexBindingDescriptions,
      .vertexAttributeDescriptionCount = vertexAttributeDescriptionCount,
      .pVertexAttributeDescriptions    = pVertexAttributeDescriptions,
  };
  return *this;
}

GraphicPipelineBuilder&
GraphicPipelineBuilder::setInputAssembly(VkPrimitiveTopology topology,
                                         VkBool32 primitiveRestartEnable) {
  this->inputAssembly = {
      .sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .pNext    = nullptr,
      .flags    = 0,
      .topology = topology,
      .primitiveRestartEnable = primitiveRestartEnable,
  };
  return *this;
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setRasterization(
    VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable,
    VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace,
    VkBool32 depthBiasEnable, float depthBiasConstantFactor,
    float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth) {
  this->rasterizer = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .depthClampEnable        = depthClampEnable,
      .rasterizerDiscardEnable = rasterizerDiscardEnable,
      .polygonMode             = polygonMode,
      .cullMode                = cullMode,
      .frontFace               = frontFace,
      .depthBiasEnable         = depthBiasEnable,
      .depthBiasConstantFactor = depthBiasConstantFactor,
      .depthBiasClamp          = depthBiasClamp,
      .depthBiasSlopeFactor    = depthBiasSlopeFactor,
      .lineWidth               = lineWidth,
  };
  return *this;
}

GraphicPipelineBuilder& GraphicPipelineBuilder::setMultisample(
    VkSampleCountFlagBits rasterizationSamples, VkBool32 sampleShadingEnable,
    float minSampleShading, const VkSampleMask* pSampleMask,
    VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable) {
  this->multisampling = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .rasterizationSamples  = rasterizationSamples,
      .sampleShadingEnable   = sampleShadingEnable,
      .minSampleShading      = minSampleShading,
      .pSampleMask           = pSampleMask,
      .alphaToCoverageEnable = alphaToCoverageEnable,
      .alphaToOneEnable      = alphaToOneEnable,
  };
  return *this;
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setColorBlendAttachment(
    VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor,
    VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp,
    VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor,
    VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask) {
  this->colorBlendAttachment = {
      .blendEnable         = blendEnable,
      .srcColorBlendFactor = srcColorBlendFactor,
      .dstColorBlendFactor = dstColorBlendFactor,
      .colorBlendOp        = colorBlendOp,
      .srcAlphaBlendFactor = srcAlphaBlendFactor,
      .dstAlphaBlendFactor = dstAlphaBlendFactor,
      .alphaBlendOp        = alphaBlendOp,
      .colorWriteMask      = colorWriteMask,
  };
  return *this;
}
GraphicPipelineBuilder&
GraphicPipelineBuilder::setTessellation(uint32_t patchControlPoints) {
  this->tessellation = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .patchControlPoints = patchControlPoints,
  };
  return *this;
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setDepthStencil(
    VkBool32 depthTestEnable, VkBool32 depthWriteEnable,
    VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable,
    VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back,
    float minDepthBounds, float maxDepthBounds) {
  this->depthStencil = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .depthTestEnable       = depthTestEnable,
      .depthWriteEnable      = depthWriteEnable,
      .depthCompareOp        = depthCompareOp,
      .depthBoundsTestEnable = depthBoundsTestEnable,
      .stencilTestEnable     = stencilTestEnable,
      .front                 = front,
      .back                  = back,
      .minDepthBounds        = minDepthBounds,
      .maxDepthBounds        = maxDepthBounds,
  };
  return *this;
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setDeepNoStencil(
    VkBool32 depthWriteEnable, VkCompareOp depthCompareOp,
    VkBool32 depthBoundsTestEnable, float minDepthBounds,
    float maxDepthBounds) {
  this->depthStencil = VkPipelineDepthStencilStateCreateInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .depthTestEnable       = VK_TRUE,
      .depthWriteEnable      = depthWriteEnable,
      .depthCompareOp        = depthCompareOp,
      .depthBoundsTestEnable = depthBoundsTestEnable,
      .stencilTestEnable     = VK_FALSE,
      .minDepthBounds        = minDepthBounds,
      .maxDepthBounds        = maxDepthBounds,
  };
  return *this;
}
GraphicPipelineBuilder& GraphicPipelineBuilder::setViewPortAndScissor(
    std::vector<VkViewport>&& viewports, std::vector<VkRect2D>&& scissor) {
  this->viewports = std::forward<decltype(viewports)>(viewports);
  this->scissors  = std::forward<decltype(scissor)>(scissor);
  return *this;
}
GraphicPipelineBuilder&
GraphicPipelineBuilder::setViewPortAndScissorCount(size_t viewportCount,
                                                   size_t scissorCount) {
  this->viewports.isInit = true;
  this->viewports.value.resize(viewportCount);
  this->viewports.value.resize(scissorCount);
  return *this;
}

VkPipeline GraphicPipelineBuilder::build(VkDevice         device,
                                         VkRenderPass     renderPass,
                                         VkPipelineLayout layout,
                                         VkPipelineCache  cache) {
  GraphicPipelineBuilder* builder = this;

  VkPipelineViewportStateCreateInfo viewportState{
      .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pNext         = nullptr,
      .flags         = 0,
      .viewportCount = u32(builder->viewports.value.size()),
      .pViewports    = builder->viewports.value.data(),
      .scissorCount  = u32(builder->scissors.value.size()),
      .pScissors     = builder->scissors.value.data(),
  };

  builder->colorBlendAttachment.required();
  
  VkPipelineColorBlendStateCreateInfo colorBlending{
      .sType         = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .pNext         = nullptr,
      .flags         = 0,
      .logicOpEnable = VK_FALSE,
      .logicOp       = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments    = &builder->colorBlendAttachment,
  };

  builder->vertexInputInfo.required();
  builder->shaders.required();
  builder->vertexInputInfo.required();
  builder->viewports.required();
  builder->rasterizer.required();
  builder->multisampling.required();
  builder->depthStencil.required();

  VkGraphicsPipelineCreateInfo pipelineCI{
      .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .pNext               = nullptr,
      .flags               = 0,
      .stageCount          = u32(shaders.value.size()),
      .pStages             = shaders.value.data(),
      .pVertexInputState   = &builder->vertexInputInfo,
      .pInputAssemblyState = &builder->inputAssembly,
      .pTessellationState =
          builder->tessellation.isInit ? &builder->tessellation : nullptr,
      .pViewportState      = &viewportState,
      .pRasterizationState = &builder->rasterizer,
      .pMultisampleState   = &builder->multisampling,
      .pDepthStencilState  = &builder->depthStencil,
      .pColorBlendState    = &colorBlending,
      .pDynamicState       = &builder->dynamic,
      .layout              = layout,
      .renderPass          = renderPass,
      .subpass             = 0,
      .basePipelineHandle  = VK_NULL_HANDLE,
  };

  VkPipeline ret;
  auto result =
      vkCreateGraphicsPipelines(device, cache, 1, &pipelineCI, nullptr, &ret);
  assert(result == VK_SUCCESS);
  return ret;
}

std::pair<VkPipeline, VkPipelineCache>
GraphicPipelineBuilder::buildWithCache(VkDevice device, VkRenderPass pass,
                                       VkPipelineLayout layout,
                                       VkPipelineCache  cache, size_t initialDataSize, void *pInitialData) {
  VkPipelineCache outCache;
  VkPipeline      outPipeline = build(device, pass, layout, cache);

  VkPipelineCacheCreateInfo CI{
      .sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
      .pNext           = nullptr,
      .flags           = 0,
      .initialDataSize = initialDataSize,
      .pInitialData    = pInitialData,
  };

  auto result = vkCreatePipelineCache(device, &CI, nullptr, &outCache);
  assert(result == VK_SUCCESS);
  return std::make_pair(outPipeline, outCache);
}
} // namespace myvk::bs