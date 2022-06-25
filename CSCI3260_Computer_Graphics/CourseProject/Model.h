#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	Model();

	void Load(const std::string&);
	void Draw(Shader&);

	void UpdateRadius(float scale) { m_Radius *= scale; }
	float GetRadius() { return m_Radius; }

private:
	void loadModel(const std::string&);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<MaterialTexture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);

	void calulateRadius();

private:
	std::vector<Mesh> m_Meshes;
	std::vector<MaterialTexture> m_TexturesLoaded;
	std::string m_Directory;

	float m_Radius;
};

