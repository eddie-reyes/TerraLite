#shader vertex
#version 410 core

uniform mat4 MVP;
		
layout(location = 0) in vec4 position;
		
void main()
{
	//multiplication order matters
	gl_Position = MVP * position;
};

#shader fragment
#version 410 core
		
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
		
void main()
{
	color = u_Color;
};