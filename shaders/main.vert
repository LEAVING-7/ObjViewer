#version 450


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNorm;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 outNorm;
layout(location = 2) out vec3 outFragPos;
layout(binding = 0) uniform MVP {
  mat4 model, view, proj;
} ubo;


void main() {
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos, 1.0);
  outFragPos = vec3(ubo.model * vec4(inPos, 1.f));
  outNorm = mat3(transpose(inverse(ubo.model))) * inNorm;
  outUV = inUV;
}
