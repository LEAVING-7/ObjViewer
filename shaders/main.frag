#version 450

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec3 inPos;

layout(location = 0) out vec4 outFragColor;

layout(binding = 1) uniform sampler2D diffuseSampler;
layout(binding = 3) uniform sampler2D specularSampler;
struct Material {
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

layout(binding = 2) uniform Light {
  vec3 position;
  vec3 color;
  vec3 camPos;
} uniLight;

vec4 phong(vec3 lightColor, vec3 lightDir, vec3 viewDir, vec3 norm, float ka, float kd, float ks, Material material) {
  float ambient = ka;
  float diffuse = kd * max(dot(norm, lightDir), 0.f);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specular = ks * pow(max(dot(viewDir, reflectDir), 0.0), 32);
  return vec4(lightColor * (ambient * material.diffuse + diffuse * material.diffuse + specular * material.specular), 1.f);
}

void main() {
  vec4 diffuseColor = texture(diffuseSampler, inTexCoord);
  vec4 specularColor = texture(specularSampler, inTexCoord);

  vec3 norm = normalize(inNorm);
  vec3 lightDir = normalize(uniLight.position - inPos);
  vec3 viewDir = normalize(uniLight.camPos - inPos);

  outFragColor = phong(uniLight.color, lightDir, viewDir, norm, 0.2f, 0.5f, 1.f, Material(vec3(diffuseColor), vec3(specularColor), 1.f));
}
