#version 460 core

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;

out vec4 FragColor;

uniform vec4 u_MaterialColor;

void main()
{
    vec3 norm = normalize( Normal_cameraspace );
    vec3 lightDir = normalize( LightDirection_cameraspace );

    float diffuse = clamp(dot(norm, lightDir), 0, 1);
    float ambient = 0.2;

    vec3 result = (ambient + diffuse) * u_MaterialColor.rgb;
    FragColor = vec4(result, u_MaterialColor.a);
}