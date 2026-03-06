#version 460 core

uniform mat4 MVP;
		
layout(location = 0) in vec4 transform;
		
void main()
{
	//multiplication order matters
	gl_Position = MVP * transform;
};