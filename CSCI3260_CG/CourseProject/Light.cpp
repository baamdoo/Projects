#include "Light.h"

Light::Light()
{
}

void Light::SetDirAmbientColor(const glm::vec3& ambientColor)
{
	m_DirLight.ambientColor = ambientColor;
}

void Light::SetDirDiffuseColor(const glm::vec3& diffuseColor)
{
	m_DirLight.diffuseColor = diffuseColor;
}

void Light::SetDirSpecularColor(const glm::vec3& specularColor)
{
	m_DirLight.specularColor = specularColor;
}

void Light::SetDirPosition(const glm::vec3& position)
{
	m_DirLight.position = position;
}

void Light::SetPointAmbientColor(const glm::vec3& ambientColor)
{
	m_PointLight.ambientColor = ambientColor;
}

void Light::SetPointDiffuseColor(const glm::vec3& diffuseColor)
{
	m_PointLight.diffuseColor = diffuseColor;
}

void Light::SetPointSpecularColor(const glm::vec3& specularColor)
{
	m_PointLight.specularColor = specularColor;
}

void Light::SetPointPosition(const glm::vec3& position)
{
	m_PointLight.position = position;
}

void Light::SetPointCoef(float constant, float linear, float quadratic)
{
	m_PointLight.constant = constant;
	m_PointLight.linear = linear;
	m_PointLight.quadratic = quadratic;
}

void Light::SetSpotAmbientColor(const glm::vec3& ambient)
{
	m_SpotLight.ambientColor = ambient;
}

void Light::SetSpotDiffuseColor(const glm::vec3& diffuse)
{
	m_SpotLight.diffuseColor = diffuse;
}

void Light::SetSpotSpecularColor(const glm::vec3& specular)
{
	m_SpotLight.specularColor = specular;
}

void Light::SetSpotPosition(const glm::vec3& position)
{
	m_SpotLight.position = position;
}

void Light::SetSpotDirection(const glm::vec3& direction)
{
	m_SpotLight.direction = direction;
}

void Light::SetSpotCutoff(float inner, float outer)
{
	m_SpotLight.innerCutoff = inner;
	m_SpotLight.outerCutoff = outer;
}

void Light::SetSpotCoef(float constant, float linear, float quadratic)
{
	m_SpotLight.constant = constant;
	m_SpotLight.linear = linear;
	m_SpotLight.quadratic = quadratic;
}


const DirLight& Light::GetDirectionalLight() const
{
	return m_DirLight;
}

const PointLight& Light::GetPointLight() const
{
	return m_PointLight;
}

const SpotLight& Light::GetSpotLight() const
{
	return m_SpotLight;
}

