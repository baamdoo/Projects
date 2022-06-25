#pragma once

#include "Dependencies/glew/glew.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

struct DirLight
{
	glm::vec3 position;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

struct PointLight
{
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;

	float innerCutoff;
	float outerCutoff;
	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

class Light
{
public:
	Light();

	void SetDirAmbientColor(const glm::vec3&);
	void SetDirDiffuseColor(const glm::vec3&);
	void SetDirSpecularColor(const glm::vec3&);
	void SetDirPosition(const glm::vec3&);

	void SetPointAmbientColor(const glm::vec3&);
	void SetPointDiffuseColor(const glm::vec3&);
	void SetPointSpecularColor(const glm::vec3&);
	void SetPointPosition(const glm::vec3&);
	void SetPointCoef(float, float, float);

	void SetSpotAmbientColor(const glm::vec3&);
	void SetSpotDiffuseColor(const glm::vec3&);
	void SetSpotSpecularColor(const glm::vec3&);
	void SetSpotPosition(const glm::vec3&);
	void SetSpotDirection(const glm::vec3&);
	void SetSpotCutoff(float, float);
	void SetSpotCoef(float, float, float);

	const DirLight& GetDirectionalLight() const;
	const PointLight& GetPointLight() const;
	const SpotLight& GetSpotLight() const;

private:
	DirLight m_DirLight;
	PointLight m_PointLight;
	SpotLight m_SpotLight;
};

