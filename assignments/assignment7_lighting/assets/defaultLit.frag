#version 450

in Surface {
    vec2 UV;
    vec3 WorldPosition;
    vec3 WorldNormal;
} fs_in;

struct Light {
    vec3 position;
    vec3 color;
};
#define MAX_LIGHTS 3
uniform Light lights[MAX_LIGHTS];
uniform int _NumLights; 

struct Material {
    float ambientK;
    float diffuseK;
    float specular;
    float shininess;
};
uniform Material material;

uniform Light _Light;
uniform mat4 _ViewProjection;  
uniform sampler2D _Texture; 

out vec4 FragColor;  

void main() {
    vec3 normal = normalize(fs_in.WorldNormal);
    vec4 viewPos = _ViewProjection * vec4(fs_in.WorldPosition, 1.0);
    vec3 toViewDir = normalize(vec3(viewPos) - fs_in.WorldPosition);

    vec3 result = vec3(0.0, 0.0, 0.0); 
    
    for(int i = 0; i < _NumLights; i++) {
        vec3 lightDir = normalize(lights[i].position - fs_in.WorldPosition);
        float distance = length(lights[i].position - fs_in.WorldPosition);
        float attenuation = 2.0 / (distance * distance);

        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = material.diffuseK * diff * lights[i].color * attenuation;

        const float specularStrength = 0.5;
        vec3 halfDir = normalize(lightDir + toViewDir);
        float spec = pow(max(dot(normal, halfDir), 0.0), material.shininess * 2.0);
        vec3 specular = material.specular * specularStrength * spec * lights[i].color * attenuation;

        vec3 ambient = material.ambientK * lights[i].color;

        result += ambient + diffuse + specular; 
    }

    vec4 texColor = texture(_Texture, fs_in.UV);
    FragColor = vec4(result, 1.0) * texColor;
}
