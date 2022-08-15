// #version 450
// layout (location = 0) in vec3 inFragColor;

// layout (location = 0) out vec4 outFragColor;

// void main()
// {
// 	outFragColor = vec4(inFragColor, 1.f);
// }
#version 450

layout (location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = vec4(1.f,0.f,0.f,1.0f);
}