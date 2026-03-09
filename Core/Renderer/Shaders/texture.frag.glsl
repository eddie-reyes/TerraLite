#version 460 core

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;

out vec4 FragColor;

uniform vec4 u_MaterialColor;

void main()
{
    vec3 N = normalize(Normal_cameraspace);
    vec3 L = normalize(LightDirection_cameraspace);

    float ndotl = max(dot(N, L), 0.0);

    // Darker bright faces
    float diffuse = pow(ndotl, 1.8);

    // Keep shadows visible
    float ambient = 0.38;

    // Slight slope lift so angled terrain still reads well
    float slopeBoost = (1.0 - ndotl) * 0.06;

    float lighting = ambient + diffuse * 0.45 + slopeBoost;
    lighting = clamp(lighting, 0.0, 0.9);

    vec3 color = u_MaterialColor.rgb * lighting;
    FragColor = vec4(color, u_MaterialColor.a);
}