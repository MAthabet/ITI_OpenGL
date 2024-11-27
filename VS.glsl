#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_color;

uniform float time;

out vec4 frag_color;
float freq1 = 100;
float freq2 = 10;
void main()
{

	float x = vertex_position.x;

	float y = time*sin((x+time) * freq1)*sin((x+time)*freq2);
	
	gl_Position = vec4(x,y/2,vertex_position.z,1);

	// for no reason
	frag_color.x = abs(vertex_position.y);
	frag_color.y = abs(vertex_position.x);
	frag_color.z = 0.5;
}