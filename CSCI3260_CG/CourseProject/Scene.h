#pragma once

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Skybox.h"

class Scene
{
public:
	enum ModelType
	{
		PLAYER,
		PLANET,
		ROCK,
		GOLD,
		STONE,
		CRAFT,
	};

	Scene();
	~Scene();

	void Create();
	void Render();

	Light* GetLight() { return m_Light; }
	Camera* GetCamera() { return m_Camera; }

	const glm::vec3& GetPosition() const { return m_Positions[ModelType::PLAYER][0]; }
	void SetPosition(const glm::vec3& position) { m_Positions[ModelType::PLAYER][0] = position; }

	const glm::vec3& GetRotation() const { return m_Rotations[ModelType::PLAYER]; }
	void SetRotation(const glm::vec3& rotation) { m_Rotations[ModelType::PLAYER] = rotation; }

	void ToggleDirLightFlag() { m_DirLightFlag = 1 - m_DirLightFlag; }
	void TogglePointLightFlag() { m_PointLightFlag = 1 - m_PointLightFlag; }

private:
	void loadModel();
	void loadTextures();
	void setupShader();
	void initializeCamera();
	void initializeLight();
	void loadSkybox();

	void renderNormalMap();
	void renderObject();
	void renderSkybox();

	void setupModelMatrices();
	void flushModelMatrices();

	void shadeDirLight(Shader*);
	void shadePointLights(Shader*);

	void updateCollisionFlags();
	bool checkCollision(const glm::mat4&, const glm::mat4&, float);

private:
	Camera* m_Camera = nullptr;
	Light* m_Light = nullptr;
	Model* m_Models = nullptr;
	Shader* m_Shader = nullptr;
	Shader* m_NormalMapShader = nullptr;
	Texture* m_Textures = nullptr;
	Texture* m_Stones = nullptr;
	Texture* m_NormalMap;
	Texture* m_Halftone = nullptr;
	Skybox* m_Skybox = nullptr;

	std::vector<glm::mat4> m_ModelMatrices[6];
	std::vector<glm::vec3> m_Positions[6];
	float m_Scales[6] = { 0.003f, 1.0f, 0.08f, 0.08f, 0.08f, 0.1f };
	glm::vec3 m_Rotations[6] = { glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };

	struct CraftInfo
	{
		int count;
		float translateValues[5];
		float boundaries[5];
		int sign[5];
	};
	CraftInfo m_CraftInfo = {
		5,
		{ 0.01f, 0.02f, 0.015f, 0.05f, 0.032f },
		{ 10.0f, 16.0f, 6.0f, 20.0f, 8.8f },
		{ 1, -1, -1, 1, -1 }
	};

	bool m_DirLightFlag = true;
	bool m_PointLightFlag = true;

	struct CollisionFlags
	{
		bool crafts[5];
		bool golds[4];
		bool stones[6];
		bool rocks[290];
	};
	CollisionFlags m_CollisionFlags = { false, };

	int m_GoldCounter = 0;
	int m_StoneCounter = 0;
	int m_AlertCounter = 0;
};

