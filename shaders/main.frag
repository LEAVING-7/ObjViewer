#version 450
layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec3 inPos;

layout(location = 0) out vec4 outFragColor;

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform Light {
  vec3 position;
  vec3 color;
} uniLight;

void main() {
  float ambientLightIntense = 0.1f;
  vec4 ambient = vec4(uniLight.color * ambientLightIntense, 1.f);

  vec3 norm = normalize(inNorm);
  vec3 lightDir = normalize(uniLight.position - inPos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec4 diffuse = diff * vec4(uniLight.color, 1);

  vec4 objectColor = texture(texSampler, inTexCoord);

  outFragColor = objectColor * (ambient + diffuse);
}
