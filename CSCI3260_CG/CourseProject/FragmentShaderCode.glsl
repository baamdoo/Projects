#version 430

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

in VS_OUT
{
	vec2 texCoords;
	vec3 normal;
	vec3 vertexPos;
} ps_in;


out vec4 outColor;
in vec2 texCoords;


uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 cameraPosition; 
uniform vec3 inputColor;

uniform sampler2D texture0;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 vertexPos, vec3 viewDir)
{
	vec3 color = texture(texture0, ps_in.texCoords).rgb;

	vec3 lightDir = normalize(light.position - vertexPos);
	float diff = clamp(dot(lightDir, normal), 0.0f, 1.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(clamp(dot(reflectDir, viewDir), 0.0f, 1.0f), 32.0f);

	vec3 ambient = light.ambientColor * color;
	vec3 diffuse = light.diffuseColor * diff * color;
	vec3 specular = light.specularColor * spec;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 vertexPos, vec3 viewDir)
{
	vec3 color = texture(texture0, ps_in.texCoords).rgb;

	vec3 lightDir = normalize(light.position - vertexPos);
	float diff = clamp(dot(lightDir, normal), 0.0f, 1.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(clamp(dot(reflectDir, viewDir), 0.0f, 1.0f), 32.0f);

	float distance = length(light.position - vertexPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambientColor * color * attenuation;
	vec3 diffuse = light.diffuseColor * diff * color * attenuation;
	vec3 specular = light.specularColor * spec * attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 normal = normalize(ps_in.normal);
	vec3 viewDir = normalize(cameraPosition - ps_in.vertexPos);

	vec4 result = vec4(CalcDirLight(dirLight, normal, ps_in.vertexPos, viewDir), 1.0f);
	result += vec4(CalcPointLight(pointLight, normal, ps_in.vertexPos, viewDir), 1.0f);
	outColor = result * vec4(inputColor, 1.0f);
}
