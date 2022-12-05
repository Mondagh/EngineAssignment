#pragma once

#include <Assimp/mesh.h>
#include <Assimp/scene.h>
#include <Assimp/cimport.h>

#include <vector>


class Mesh
{
public:
	Mesh();
	~Mesh();
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);
	unsigned vbo;
	unsigned ebo;
	unsigned vao;

	
	aiMesh* GetAiMeshe();
	void SetAiMesh(aiMesh* mesh);
	void SetMaterialIndex(const unsigned int numMaterialIndex);
	int GetNumVertices();
	int GetNumIndices();

private:
	aiMesh* aiMeshe;
	int num_vertices;
	int num_indices;
	unsigned int materialIndex;
};

