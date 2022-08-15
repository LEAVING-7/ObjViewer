#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {
struct GraphicPipelineBuilder {

  template <typename T> struct Init {
    bool isInit{false};
    T    value;

    Init()            = default;
    Init(const Init&) = delete;
    Init(Init&&)      = delete;

    T& operator=(T const& another) {
      isInit = true;
      value  = another;
      return value;
    }
    T& operator==(T&& another) {
      isInit = true;
      value  = std::forward<T&&>(another);
      return value;
    }
    T* operator&() {
      required();
      return &value;
    }
    operator bool() {
      return isInit;
    }

    void required() {
      assert(isInit);
    }
  };

  Init<std::vector<VkPipelineShaderStageCreateInfo>> shaders;

  Init<VkPipelineDynamicStateCreateInfo>       dynamic;
  Init<VkPipelineVertexInputStateCreateInfo>   vertexInputInfo;
  Init<VkPipelineInputAssemblyStateCreateInfo> inputAssembly;

  Init<std::vector<VkViewport>> viewports;
  Init<std::vector<VkRect2D>>   scissors;

  Init<VkPipelineRasterizationStateCreateInfo> rasterizer;
  Init<VkPipelineColorBlendAttachmentState>    colorBlendAttachment;

  Init<VkPipelineTessellationStateCreateInfo> tessellation;
  Init<VkPipelineMultisampleStateCreateInfo>  multisampling;
  Init<VkPipelineDepthStencilStateCreateInfo> depthStencil;

  VkPipeline build(VkDevice device, VkRenderPass pass, VkPipelineLayout layout,
                   GraphicPipelineBuilder* builder = nullptr,
                   VkPipelineCache         cache   = VK_NULL_HANDLE);

  GraphicPipelineBuilder&
  setShader(std::vector<VkPipelineShaderStageCreateInfo>&& shaders);

  GraphicPipelineBuilder& noColorBlend(VkColorComponentFlags colorWriteMask);

  // TODO 重构成vector
  GraphicPipelineBuilder&
  setDynamic(uint32_t              dynamicStateCount = 0,
             const VkDynamicState* pDynamicStates    = nullptr);

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
  setLayout(uint32_t setLayoutCount, const VkDescriptorSetLayout* pSetLayouts,
            uint32_t                   pushConstantRangeCount,
            const VkPushConstantRange* pPushConstantRanges);

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
  setViewPortAndScissor(std::vector<VkViewport>&& viewports,
                        std::vector<VkRect2D>&&   scissors);
  GraphicPipelineBuilder& setViewPortAndScissorCount(size_t viewportCount,
                                                     size_t scissorCount);
};

} // namespace myvk_bs