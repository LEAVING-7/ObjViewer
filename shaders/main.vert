#version 450

// layout(location = 0) in vec2 inPosition;
// layout(location = 1) in vec3 inColor;

// layout(location = 0) out vec3 outFragColor;

// void main()
// {
//   gl_Position = vec4(inPosition, 0.0, 1.0);
//   outFragColor = inColor;
// }


void main()
{
	//const array of positions for the triangle
	const vec3 positions[3] = vec3[3](
		vec3(1.f,1.f, 0.0f),
		vec3(-1.f,1.f, 0.0f),
		vec3(0.f,-1.f, 0.0f)
	);
  
	//output the position of each vertex
	gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
}