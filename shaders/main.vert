#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 outFragColor;


layout(binding = 0) uniform MVP {
  mat4 model, view, proj;
} ubo;

layout(push_constant) uniform PushConstant {
  mat4 model, view, proj;
} pushConstants;

void main()
{
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
  outFragColor = inColor;
}

