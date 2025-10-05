#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;       
layout (location = 1) in vec3 aNormal;    
layout (location = 2) in vec3 aTangent;   
layout (location = 3) in vec2 aTexCoord;  

out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
    FragPos  = vec3(model * vec4(aPos, 1.0));

    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal  = normalize(normalMatrix * aNormal);
    Tangent = normalize(normalMatrix * aTangent);
}

#shader pixel
#version 460 core

struct MaterialS {
    vec3 Color;
    float Intensity;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct DirectionalLight {
    vec3 Color;
    float Intensity;
    vec3 Direction;
};

struct PointLight {
    vec3 Color;
    float Intensity;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
};

struct SpotLight {
    vec3 Color;
    float Intensity;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
    vec3 Direction;
    float CutOff;
    float OuterCutOff;
};

#define MAX_DIRECTIONAL_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

uniform int NumDirectionalLights;
uniform int NumPointLights;
uniform int NumSpotLights;

uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform SpotLight SpotLights[MAX_SPOT_LIGHTS];

uniform vec3 ViewPos;
uniform MaterialS Material;

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);

    vec3 ambient  = light.Color * light.Intensity * Material.Ambient;
    vec3 diffuse  = light.Color * light.Intensity * diff * Material.Diffuse;
    vec3 specular = light.Color * light.Intensity * spec * Material.Specular;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);

    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance +
                               light.Quadratic * (distance * distance));

    vec3 ambient  = light.Color * light.Intensity * Material.Ambient;
    vec3 diffuse  = light.Color * light.Intensity * diff * Material.Diffuse;
    vec3 specular = light.Color * light.Intensity * spec * Material.Specular;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);

    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance +
                               light.Quadratic * (distance * distance));

    float theta     = dot(lightDir, normalize(-light.Direction));
    float epsilon   = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.Color * light.Intensity * Material.Ambient;
    vec3 diffuse  = light.Color * light.Intensity * diff * Material.Diffuse;
    vec3 specular = light.Color * light.Intensity * spec * Material.Specular;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < NumDirectionalLights; ++i) {
        result += CalcDirectionalLight(DirectionalLights[i], norm, viewDir);
    }
    for (int i = 0; i < NumPointLights; ++i) {
        result += CalcPointLight(PointLights[i], norm, FragPos, viewDir);
    }
    for (int i = 0; i < NumSpotLights; ++i) {
        result += CalcSpotLight(SpotLights[i], norm, FragPos, viewDir);
    }

    // --- ACES Filmic Tone Mapping ---
    // TODO: create separate post-processing shader
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;

    vec3 toneMapped = clamp((result * (a * result + b)) / (result * (c * result + d) + e), 0.0, 1.0);

    FragColor = vec4(toneMapped, 1.0);
}