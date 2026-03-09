#version 460 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec4 u_MaterialColor;
uniform vec3 u_LightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDir = vec3(0.0, -1.0, 0.0);

		
void main()
{
	// Normalize interpolated normal
    vec3 norm = normalize(Normal);

    // For diffuse lighting, we want the direction FROM the fragment TO the light,
    // which is the opposite of the light ray direction.
    vec3 toLight = normalize(-lightDir);

    // Ambient term
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_LightColor;

    // Diffuse term
    float diff = max(dot(norm, toLight), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Final color
    vec3 result = (ambient + diffuse) * u_MaterialColor.xyz;
    FragColor = vec4(result, 1.0);
};