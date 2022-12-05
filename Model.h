#pragma once

#include "Mesh.h"

#include <Assimp/mesh.h>
#include <Assimp/scene.h>
#include <Assimp/cimport.h>
#include <Assimp/postprocess.h>
#include "GL/glew.h"

#include <vector>
#include<list>

class Model
{
public:
	Model();
	~Model();
	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(aiMesh** mMesh, int mNumMeshes);
	void Draw();

	std::vector<GLuint> materials;
	std::list<Mesh*> meshes;
	Mesh* mmMesh;
	int GetNumVertices();
	int GetNumIndices();
};

