#include "Scene.h"

const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

Scene::Scene()
{
	m_Positions[ModelType::PLAYER].push_back(glm::vec3(0.0f));
	m_Positions[ModelType::PLANET].push_back(glm::vec3(1.0f, 1.0f, -50.0f));
	m_Positions[ModelType::ROCK].push_back(glm::vec3(1.0f, 1.7f, -50.0f));
	m_Positions[ModelType::GOLD].push_back(glm::vec3(1.0f, 1.7f, -50.0f));
	m_Positions[ModelType::STONE].push_back(glm::vec3(1.0f, 1.7f, -50.0f));
	for (int i = 0; i < m_CraftInfo.count; i++)
		m_Positions[ModelType::CRAFT].push_back(glm::vec3(0.0f, 1.2f, -1.0f));
}
Scene::~Scene()
{
}

void Scene::Create()
{
	loadModel();
	loadTextures();
	setupShader();
	initializeCamera();
	initializeLight();
	loadSkybox();
}
void Scene::Render()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupModelMatrices();
	updateCollisionFlags();

	renderNormalMap();
	renderObject();
	renderSkybox();

	flushModelMatrices();
}

void Scene::loadModel()
{
	std::string objects[] = {
		"./resources/objects/spacecraft.obj",
		"./resources/objects/planet.obj",
		"./resources/objects/rock.obj",
		"./resources/objects/rock.obj",
		"./resources/objects/stone.obj",
		"./resources/objects/craft.obj",
	};

	m_Models = new Model[6];
	for (int i = 0; i < 6; i++)
	{
		m_Models[i].Load(objects[i]);
		m_Models[i].UpdateRadius(m_Scales[i]);
	}
}
void Scene::loadTextures()
{
	const char* texturePaths[] =
	{
		"./resources/textures/spacecraftTexture.bmp",
		"./resources/textures/earthTexture.bmp",
		"./resources/textures/rockTexture.bmp",
		"./resources/textures/gold_01.jpg",
		"./resources/textures/power.jpg",
		"./resources/textures/ringTexture.bmp",
	};
	const char* stonePaths[] =
	{
		"./resources/textures/space.jpg",
		"./resources/textures/mind.jpg",
		"./resources/textures/reality.jpg",
		"./resources/textures/power.jpg",
		"./resources/textures/time.jpg",
		"./resources/textures/soul.jpg",
	};

	m_Textures = new Texture[6];
	m_Stones = new Texture[6];
	m_NormalMap = new Texture[2];
	m_Halftone = new Texture[1];
	for (int i = 0; i < 6; i++)
	{
		m_Textures[i].setupTexture2D(texturePaths[i]);
		m_Stones[i].setupTexture2D(stonePaths[i]);
	}
	m_NormalMap[0].setupTexture2D("./resources/textures/earthNormal.bmp");
	m_NormalMap[1].setupTexture2D("./resources/textures/stoneNormal.jpeg");
	m_Halftone[0].setupTexture2D("./resources/textures/halftone.jpeg");

}
void Scene::setupShader()
{
	m_Shader = new Shader;
	m_NormalMapShader = new Shader;
	m_Shader->setupShader("./VertexShaderCode.glsl", "./FragmentShaderCode.glsl");
	m_NormalMapShader->setupShader("./NormalMapVS.glsl", "./NormalMapPS.glsl");
}
void Scene::initializeCamera()
{
	m_Camera = new Camera;
	m_Camera->SetPosition(glm::vec3(0.0f, 3.0f, 10.0f));
}
void Scene::initializeLight()
{
	m_Light = new Light;

	m_Light->SetDirAmbientColor(glm::vec3(0.1f, 0.1f, 0.1f));
	m_Light->SetDirDiffuseColor(glm::vec3(0.876f, 0.654f, 0.210f));
	m_Light->SetDirSpecularColor(glm::vec3(0.876f, 0.654f, 0.210f));
	m_Light->SetDirPosition(glm::vec3(1.0f, 1.5f, 5.0f));

	m_Light->SetPointAmbientColor(glm::vec3(0.1f, 0.1f, 0.1f));
	m_Light->SetPointDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_Light->SetPointSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_Light->SetPointPosition(glm::vec3(0.0f, 10.0f, -20.0f));
	m_Light->SetPointCoef(1.0f, 0.007f, 0.0002f);

	m_Light->SetSpotAmbientColor(glm::vec3(0.0f, 0.0f, 0.0f));
	m_Light->SetSpotDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));
	m_Light->SetSpotSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));
	m_Light->SetSpotPosition(m_Camera->GetPosition());
	m_Light->SetSpotDirection(m_Camera->GetDirection());
	m_Light->SetSpotCoef(1.0f, 0.09f, 0.032f);
	m_Light->SetSpotCutoff(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
}
void Scene::loadSkybox()
{
	m_Skybox = new Skybox;
	const char* texturePaths[] =
	{
		"./resources/skybox/bkg1_right.png",
		"./resources/skybox/bkg1_left.png",
		"./resources/skybox/bkg1_top.png",
		"./resources/skybox/bkg1_bot.png",
		"./resources/skybox/bkg1_front.png",
		"./resources/skybox/bkg1_back.png"
	};
	m_Skybox->SetupSkybox(texturePaths);
}

void Scene::renderNormalMap()
{
	m_NormalMapShader->use();

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 projectionMatrix = m_Camera->GetProjectionMatrix();

	m_NormalMapShader->setMat4("viewMatrix", viewMatrix);
	m_NormalMapShader->setMat4("projectionMatrix", projectionMatrix);
	m_NormalMapShader->setVec3("pointLightPos", m_Light->GetPointLight().position);
	m_NormalMapShader->setVec3("cameraPos", m_Camera->GetPosition());
	m_NormalMapShader->setInt("texture0", 0);
	m_NormalMapShader->setInt("normalMap", 1);

	shadeDirLight(m_NormalMapShader);
	shadePointLights(m_NormalMapShader);
	
	
	if (m_StoneCounter < m_ModelMatrices[ModelType::STONE].size())
		m_Textures[ModelType::PLANET].bind2D(0);
	else
		m_Halftone[0].bind2D(0);
	m_NormalMap[0].bind2D(1);
	m_NormalMapShader->setMat4("modelMatrix", m_ModelMatrices[ModelType::PLANET].front());
	m_Models[ModelType::PLANET].Draw(*m_NormalMapShader);

	m_NormalMap[1].bind2D(1);
	for (int i = 0; i < m_ModelMatrices[ModelType::STONE].size(); i++)
	{
		if (m_CollisionFlags.stones[i])
			continue;
		m_Stones[i].bind2D(0);
		m_NormalMapShader->setMat4("modelMatrix", m_ModelMatrices[ModelType::STONE][i]);
		m_Models[ModelType::STONE].Draw(*m_NormalMapShader);
	}
}
void Scene::renderObject()
{
	m_Shader->use();

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 projectionMatrix = m_Camera->GetProjectionMatrix();
	glm::vec3 color = glm::vec3(1.0f);

	m_Shader->setMat4("viewMatrix", viewMatrix);
	m_Shader->setMat4("projectionMatrix", projectionMatrix);
	m_Shader->setVec3("cameraPosition", m_Camera->GetPosition());
	m_Shader->setInt("texture0", 0);

	shadeDirLight(m_Shader);
	shadePointLights(m_Shader);

	for (int i = 0; i < 6; i++)
	{
		if (i == ModelType::PLANET || i == ModelType::STONE)
			continue;

		m_Textures[i].bind2D(0);
		for (int j = 0; j < m_ModelMatrices[i].size(); j++)
		{
			if (i == ModelType::PLAYER && m_GoldCounter == m_ModelMatrices[ModelType::GOLD].size())
			{
				m_Textures[ModelType::GOLD].bind2D(0);
			}
			else if (i == ModelType::CRAFT && m_CollisionFlags.crafts[j])
			{
				m_AlertCounter = (m_AlertCounter + 1) % 100;
				if (m_AlertCounter > 50)
					color = glm::vec3(1.0f);
				else
					color = glm::vec3(1.0f, 0.0f, 0.0f);
			}
			else if (i == ModelType::ROCK && m_CollisionFlags.rocks[j])
			{
				color = glm::vec3(0.1f, 0.1f, 0.1f);
			}
			else if (i == ModelType::GOLD && m_CollisionFlags.golds[j])
			{
				continue;
			}
			m_Shader->setVec3("inputColor", color);
			m_Shader->setMat4("modelMatrix", m_ModelMatrices[i][j]);
			m_Models[i].Draw(*m_Shader);

			color = glm::vec3(1.0f);
		}
	}
}
void Scene::renderSkybox()
{
	glDepthFunc(GL_LEQUAL);
	m_Skybox->GetSkyboxShader()->use();
	m_Skybox->GetSkyboxShader()->setInt("skybox", 0);

	glm::mat4 viewMatrix = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));
	glm::mat4 projectionMatrix = m_Camera->GetProjectionMatrix();
	m_Skybox->GetSkyboxShader()->setMat4("viewMatrix", viewMatrix);
	m_Skybox->GetSkyboxShader()->setMat4("projectionMatrix", projectionMatrix);

	glBindVertexArray(m_Skybox->GetSkyboxVaoID());
	m_Skybox->BindTexture(0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Scene::setupModelMatrices()
{
	glm::mat4 matrix;

	// Setup Player location
	matrix = glm::inverse(m_Camera->GetViewMatrix())
		* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -5.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotations[ModelType::PLAYER].y), glm::vec3(0, 1, 0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scales[ModelType::PLAYER]));
	m_ModelMatrices[ModelType::PLAYER].push_back(matrix);
	


	// Setup Planet location
	m_Rotations[ModelType::PLANET].y += 0.1f;
	if (m_Rotations[ModelType::PLANET].y > 360.0f)
		m_Rotations[ModelType::PLANET].y = 0.0f;
	matrix = glm::translate(glm::mat4(1.0f), m_Positions[ModelType::PLANET].front())
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotations[ModelType::PLANET].y), glm::vec3(0, 1, 0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scales[ModelType::PLANET]));
	m_ModelMatrices[ModelType::PLANET].push_back(matrix);


	// Setup Rocks and golds location
	matrix = glm::translate(glm::mat4(1.0f), m_Positions[ModelType::ROCK].front());
	m_Rotations[ModelType::ROCK].y += 0.02f;
	m_Rotations[ModelType::GOLD].y += 0.01f;
	m_Rotations[ModelType::STONE].y += 0.01f;
	if (m_Rotations[ModelType::ROCK].y > 360.0f)
		m_Rotations[ModelType::ROCK].y = 0.0f;
	if (m_Rotations[ModelType::GOLD].y > 360.0f)
		m_Rotations[ModelType::GOLD].y = 0.0f;
	if (m_Rotations[ModelType::STONE].y > 360.0f)
		m_Rotations[ModelType::STONE].y = 0.0f;
	float scale, translateTheta, translateY;
	for (int i = 0; i < 300; i++)
	{
		// Randomize scale of rocks and golds
		srand(i * 3260);
		int rN = rand() % 100;
		scale = (float)rN / 100.0f;
		if (i < 4)
			scale = glm::max(scale, 0.5f);
		if (i < 10)
			scale = 2.0f;

		// Randomize location of rocks and golds
		srand(i * 795);
		rN = rand() % 360;
		translateTheta = (float)rN;
		translateY = ((float)rand() / (float)RAND_MAX) / 2;
		int sign = (rand() % 2) - 1;
		float radius = 5.0f - ((float)rand() / (float)RAND_MAX);
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(radius * sin(glm::radians(translateTheta)), sign * translateY, radius * cos(glm::radians(translateTheta))));

		// Randomize angle of rocks and golds
		srand(i * 1996);
		float theta;
		rN = rand() % 360;
		theta = (float)rN;
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(0, 0, 1));

		if (i < 4)
		{
			glm::mat4 tmpMatrix = matrix
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotations[ModelType::GOLD].y), glm::vec3(0, 1, 0))
				* translation * rotation
				* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scales[ModelType::GOLD] * scale));
			m_ModelMatrices[ModelType::GOLD].push_back(tmpMatrix);
		}
		else if (i < 10)
		{
			glm::mat4 tmpMatrix = matrix
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotations[ModelType::STONE].y), glm::vec3(0, 1, 0))
				* translation * rotation
				* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scales[ModelType::STONE] * scale));
			m_ModelMatrices[ModelType::STONE].push_back(tmpMatrix);
		}
		else
		{
			glm::mat4 tmpMatrix = matrix
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotations[ModelType::ROCK].y), glm::vec3(0, 1, 0))
				* translation * rotation
				* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scales[ModelType::ROCK] * scale));
			m_ModelMatrices[ModelType::ROCK].push_back(tmpMatrix);
		}
	}


	// Setup Crafts location
	m_Rotations[ModelType::CRAFT].y += 0.5f;
	if (m_Rotations[ModelType::CRAFT].y > 360.0f)
		m_Rotations[ModelType::CRAFT].y = 0.0f;
	for (int j = 0; j < m_CraftInfo.count; j++)
	{
		m_Positions[ModelType::CRAFT][j].x += m_CraftInfo.translateValues[j] * m_CraftInfo.sign[j];
		if (abs(m_Positions[ModelType::CRAFT][j].x) > m_CraftInfo.boundaries[j])
			m_CraftInfo.sign[j] = -m_CraftInfo.sign[j];

		matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -8.0f * j))
			* glm::translate(glm::mat4(1.0f), m_Positions[ModelType::CRAFT][j])
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotations[ModelType::CRAFT].y), glm::vec3(0, 1, 0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scales[ModelType::CRAFT]));
		m_ModelMatrices[ModelType::CRAFT].push_back(matrix);
	}
}
void Scene::flushModelMatrices()
{
	for (int i = 0; i < 6; i++)
		m_ModelMatrices[i].clear();
}

void Scene::shadeDirLight(Shader* shader)
{
	DirLight dirLight = m_Light->GetDirectionalLight();
	glm::vec3 vNull = glm::vec3(0.0f);

	if (m_DirLightFlag)
	{
		shader->setVec3("dirLight.ambientColor", dirLight.ambientColor);
		shader->setVec3("dirLight.diffuseColor", dirLight.diffuseColor);
		shader->setVec3("dirLight.specularColor", dirLight.specularColor);
		shader->setVec3("dirLight.position", dirLight.position);
	}
	else
	{
		shader->setVec3("dirLight.ambientColor", vNull);
		shader->setVec3("dirLight.diffuseColor", vNull);
		shader->setVec3("dirLight.specularColor", vNull);
		shader->setVec3("dirLight.position", vNull);
	}
}
void Scene::shadePointLights(Shader* shader)
{
	PointLight pointLight = m_Light->GetPointLight();
	glm::vec3 vNull = glm::vec3(0.0f);
		
	std::string str = "pointLight.";
	if (m_PointLightFlag)
	{
		shader->setVec3((str + "ambientColor").c_str(), pointLight.ambientColor);
		shader->setVec3((str + "diffuseColor").c_str(), pointLight.diffuseColor);
		shader->setVec3((str + "specularColor").c_str(), pointLight.specularColor);
		shader->setVec3((str + "position").c_str(), pointLight.position);
		shader->setFloat((str + "constant").c_str(), pointLight.constant);
		shader->setFloat((str + "linear").c_str(), pointLight.linear);
		shader->setFloat((str + "quadratic").c_str(), pointLight.quadratic);
	}
	else
	{
		shader->setVec3((str + "ambientColor").c_str(), vNull);
		shader->setVec3((str + "diffuseColor").c_str(), vNull);
		shader->setVec3((str + "specularColor").c_str(), vNull);
		shader->setVec3((str + "position").c_str(), vNull);
		shader->setFloat((str + "constant").c_str(), pointLight.constant);
		shader->setFloat((str + "linear").c_str(), pointLight.linear);
		shader->setFloat((str + "quadratic").c_str(), pointLight.quadratic);
	}
}

void Scene::updateCollisionFlags()
{
	int index = 0;
	for (int i = 2; i < 6; i++)
	{
		float threshold = m_Models[ModelType::PLAYER].GetRadius() + m_Models[i].GetRadius();
		for (int j = 0; j < m_ModelMatrices[i].size(); j++)
		{
			switch (i)
			{
			case ModelType::CRAFT:
				if (m_CollisionFlags.crafts[j])
					continue;
				m_CollisionFlags.crafts[j] = checkCollision(m_ModelMatrices[ModelType::PLAYER].front(), m_ModelMatrices[i][j], threshold);
				break;
			case ModelType::GOLD:
				if (m_CollisionFlags.golds[j])
					continue;
				m_CollisionFlags.golds[j] = checkCollision(m_ModelMatrices[ModelType::PLAYER].front(), m_ModelMatrices[i][j], threshold);
				if (m_CollisionFlags.golds[j])
					m_GoldCounter++;
				break;
			case ModelType::STONE:
				if (m_CollisionFlags.stones[j])
					continue;
				m_CollisionFlags.stones[j] = checkCollision(m_ModelMatrices[ModelType::PLAYER].front(), m_ModelMatrices[i][j], threshold);
				if (m_CollisionFlags.stones[j])
					m_StoneCounter++;
				break;
			case ModelType::ROCK:
				if (m_CollisionFlags.rocks[j])
					continue;
				m_CollisionFlags.rocks[j] = checkCollision(m_ModelMatrices[ModelType::PLAYER].front(), m_ModelMatrices[i][j], threshold);
				break;
			}
			
		}
	}
}
bool Scene::checkCollision(const glm::mat4& matrix1, const glm::mat4& matrix2, float threshold)
{
	glm::vec4 vec1 = matrix1 * glm::vec4(0, 0, 0, 1);
	glm::vec4 vec2 = matrix2 * glm::vec4(0, 0, 0, 1);

	float length = glm::length(vec1 - vec2);
	if (length < threshold * 0.5f)
		return true;
	else
		return false;
}
