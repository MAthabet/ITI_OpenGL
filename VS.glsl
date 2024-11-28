#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_color;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec4 frag_color;

void main()
{
	//gl_Position = vertex_position;

	float x = vertex_position.x/6;
	float y = vertex_position.y/6;
	float z = vertex_position.z/6;

	gl_Position = projMat * viewMat * modelMat * vec4(x,y,z,1);
//	x = gl_Position.x;
//	y = gl_Position.y;
//	x = gl_Position.z;
	frag_color = x+y+z < 0? vec4(212.0f/255,199.0f/255,188.0f/255,1):vec4(126.0f/255,69.0f/255,52.0f/255,1);
}