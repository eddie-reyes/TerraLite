#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
		
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1);

    vec3 Position_worldspace = (model * vec4(aPos,1)).xyz;

    vec3 vertexPosition_cameraspace = ( view * model * vec4(aPos, 1)).xyz;
    vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    vec3 LightPosition_cameraspace = ( view * vec4(vec3(0, 0, -1),1)).xyz;
    vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    vec3 Normal_cameraspace = ( view * model * vec4(aPos, 0)).xyz;


};