#version 430 core

struct DirLight
{
	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

out vec4 outColor;

in VS_OUT {
    vec3 vertexPos;
    vec2 texCoords;
    vec3 tangentDirLightPos;
    vec3 tangentPointLightPos;
    vec3 tangentViewPos;
    vec3 tangentVertexPos;
} ps_in;

uniform sampler2D texture0;
uniform sampler2D normalMap;

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 cameraPos;

void main()
{
    vec3 color = texture(texture0, ps_in.texCoords).rgb;

    vec3 normal = texture(normalMap, ps_in.texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

	// Point light
    float distance = length(ps_in.tangentPointLightPos - ps_in.tangentVertexPos);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
   
    vec3 lightDir = normalize(ps_in.tangentPointLightPos - ps_in.tangentVertexPos);
    float diff = clamp(dot(lightDir, normal), 0.0f, 1.0f);
    
    vec3 viewDir = normalize(ps_in.tangentViewPos - ps_in.tangentVertexPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0f), 32.0f);

    vec3 point_ambient = pointLight.ambientColor * color * attenuation;
    vec3 point_diffuse = pointLight.diffuseColor * diff * color * attenuation;
    vec3 point_specular = pointLight.specularColor * spec * attenuation;

	
	// Directional light
	lightDir = normalize(ps_in.tangentDirLightPos  - ps_in.tangentVertexPos);
	diff = clamp(dot(lightDir, normal), 0.0f, 1.0f);

	viewDir = normalize(ps_in.tangentViewPos - ps_in.tangentVertexPos);
	reflectDir = reflect(-lightDir, normal);
	spec = pow(clamp(dot(reflectDir, viewDir), 0.0f, 1.0f), 32.0f);

	vec3 dir_ambient = dirLight.ambientColor * color;
	vec3 dir_diffuse = dirLight.diffuseColor * diff * color;
	vec3 dir_specular = dirLight.specularColor * spec * color;


	// Result
	vec3 ambient = point_ambient + dir_ambient;
	vec3 diffuse = point_diffuse + dir_diffuse;
	vec3 specular = point_specular + dir_specular;

    outColor = vec4(ambient + diffuse + specular, 1.0f);
}