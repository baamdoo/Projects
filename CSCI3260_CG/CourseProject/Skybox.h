#pragma once

#include "Texture.h"
#include "Shader.h"

class Skybox
{
public:
	void SetupSkybox(const char* texturePaths[]);
	void BindTexture(unsigned int slot);

	GLuint GetSkyboxVaoID() { return m_SkyboxVaoID; }
	Shader* GetSkyboxShader() { return m_SkyboxShader; }

private:
	void CreateCube();
	void SetupShader();
	void SetupTexture(const char* texturePaths[]);

private:
	GLuint m_SkyboxVaoID;

	Texture* m_SkyboxTexture;
	Shader* m_SkyboxShader;
};

