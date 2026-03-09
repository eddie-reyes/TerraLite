#version 460 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec4 u_MaterialColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0) - FragPos);

    float NdotL = max(dot(norm, lightDir), 0.0);
    float ambient = 0.2;
    float diffuse = 0.5 * NdotL;

    vec3 result = (ambient + diffuse) * u_MaterialColor.rgb;
    FragColor = vec4(result, u_MaterialColor.a);
}