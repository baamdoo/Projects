#pragma once

#include "Texture.h"
#include "Shader.h"

class ShadowMap
{
public:
	void SetupShadowMap();

	void BindTexture(unsigned int slot) const;

	const glm::mat4& GetDepthMVP() const { return m_DepthMVP; }
	GLuint GetShadowMapFramebufferID() { return m_ShadowMapFramebufferID; }
	Shader* GetShadowMapShader() { return m_ShadowShader; }

private:
	void SetupShader();
	void SetupTexture();

private:
	GLuint m_ShadowMapFramebufferID;
	Texture* m_ShadowTexture;
	Shader* m_ShadowShader;

	glm::mat4 m_DepthMVP;
};

