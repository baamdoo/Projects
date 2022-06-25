#include "ShadowMap.h"

void ShadowMap::SetupShadowMap()
{
	SetupShader();
	SetupTexture();
}

void ShadowMap::BindTexture(unsigned int slot = 0) const
{
	m_ShadowTexture->bind2D(slot);
}

void ShadowMap::SetupShader()
{
	m_ShadowShader = new Shader;
	m_ShadowShader->setupShader("ShadowVS.glsl", "ShadowPS.glsl");
}
void ShadowMap::SetupTexture()
{
	m_ShadowTexture = new Texture;
	// m_ShadowTexture->setupTexture2D();

	glGenFramebuffers(1, &m_ShadowMapFramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFramebufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowTexture->GetID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer does not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}