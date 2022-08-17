#pragma once
#include "common.hpp"
#include "pch.hpp"

#include "DataType/Vertex.hpp"
namespace myvk::bs {
struct GraphicPipelineBuilder {

  std::vector<VkPipelineShaderStageCreateInfo> shaders;
  std::vector<VkDynamicState>                  dynamicStates;

  data::VertexInputDescription           vertexInput;
  VkPipelineInputAssemblyStateCreateInfo inputAssembly;

  std::vector<VkViewport> viewports;
  std::vector<VkRect2D>   scissors;

  VkPipelineRasterizationStateCreateInfo rasterizer;
  VkPipelineColorBlendAttachmentState    colorBlendAttachment;

  VkPipelineTessellationStateCreateInfo tessellation;
  VkPipelineMultisampleStateCreateInfo  multisampling;
  VkPipelineDepthStencilStateCreateInfo depthStencil;

  VkPipeline build(VkDevice device, VkRenderPass pass, VkPipelineLayout layout,
                   bool            enableTessellation = false,
                   VkPipelineCache cache              = VK_NULL_HANDLE);

  std::pair<VkPipeline, VkPipelineCache>
  buildWithCache(VkDevice device, VkRenderPass pass, VkPipelineLayout layout,
                 VkPipelineCache cache  = VK_NULL_HANDLE,
                 size_t initialDataSize = 0, void* pInitialData = nullptr);

  GraphicPipelineBuilder&
  setShader(std::vector<VkPipelineShaderStageCreateInfo>&& shaders) {
    this->shaders = std::move(shaders);
    return *this;
  }

  GraphicPipelineBuilder&
  setShader(std::vector<VkPipelineShaderStageCreateInfo>& shaders) {
    this->shaders = shaders;
    return *this;
  }

  GraphicPipelineBuilder& noColorBlend(VkColorComponentFlags colorWriteMask);

  GraphicPipelineBuilder&
  setDynamic(uint32_t              dynamicStateCount = 0,
             const VkDynamicState* pDynamicStates    = nullptr);
  GraphicPipelineBuilder&
  setDynamic(std::vector<VkDynamicState>&& dynamicState);

  GraphicPipelineBuilder&
  setVertexInput(data::VertexInputDescription&& vertDesc);

  GraphicPipelineBuilder& setVertexInput(
      u32                                      vertexBindingDescriptionCount,
      const VkVertexInputBindingDescription*   pVertexBindingDescriptions,
      u32                                      vertexAttributeDescriptionCount,
      const VkVertexInputAttributeDescription* pVertexAttributeDescriptions);

  GraphicPipelineBuilder& setInputAssembly(VkPrimitiveTopology topology,
                                           VkBool32 primitiveRestartEnable);

  GraphicPipelineBuilder&
  setRasterization(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable,
                   VkPolygonMode polygonMode, VkCullModeFlags cullMode,
                   VkFrontFace frontFace, VkBool32 depthBiasEnable,
                   float depthBiasConstantFactor, float depthBiasClamp,
                   float depthBiasSlopeFactor, float lineWidth);

  GraphicPipelineBuilder&
  setMultisample(VkSampleCountFlagBits rasterizationSamples,
                 VkBool32 sampleShadingEnable, float minSampleShadin,
                 const VkSampleMask* pSampleMask,
                 VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnabl);

  GraphicPipelineBuilder& setColorBlendAttachment(
      VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor,
      VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp,
      VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor,
      VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask);

  GraphicPipelineBuilder& setTessellation(uint32_t patchControlPoints);

  GraphicPipelineBuilder&
  setDepthStencil(VkBool32 depthTestEnable, VkBool32 depthWriteEnable,
                  VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable,
                  VkBool32 stencilTestEnable, VkStencilOpState front,
                  VkStencilOpState back, float minDepthBounds,
                  float maxDepthBounds);
  GraphicPipelineBuilder& setDeepNoStencil(VkBool32    depthWriteEnable,
                                           VkCompareOp depthCompareOp,
                                           VkBool32    depthBoundsTestEnable,
                                           float       minDepthBounds,
                                           float       maxDepthBounds);

  GraphicPipelineBuilder&
  setViewPortAndScissor(std::vector<VkViewport>& viewports,
                        std::vector<VkRect2D>&   scissors) {
    this->viewports = viewports;
    this->scissors  = scissors;
    return *this;
  }

  GraphicPipelineBuilder&
  setViewPortAndScissor(std::vector<VkViewport>&& viewports,
                        std::vector<VkRect2D>&&   scissors) {
    this->viewports = std::move(viewports);
    this->scissors  = std::move(scissors);
    return *this;
  }

  GraphicPipelineBuilder& setViewPortAndScissorCount(size_t viewportCount,
                                                     size_t scissorCount) {
    this->viewports.resize(viewportCount);
    this->scissors.resize(scissorCount);
    return *this;
  }
};

} // namespace myvk::bs