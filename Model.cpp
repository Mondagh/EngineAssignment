#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Mesh.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "Globals.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "iostream"

//#include "Math/float2.h"




using namespace std;


Model::Model()
{
}

Model::~Model()
{
}

void Model::Load(const char* file_name)
{
	std::cout << "Num Materials: " << "\n";
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		 
	}
	else
	{
		//LOG("error loading %s: %s", file_name, aiGetErrorString()); 
	}

}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			materials.push_back(App->texture->Load(file.data));
			
		}
	}
}

void Model::LoadMeshes(aiMesh** mMesh, int mNumMeshes)
{
	for (unsigned int i = 0; i < mNumMeshes; i++)
	{
		Mesh* mesh = new Mesh();
		aiMesh*  aiMesh = mMesh[i];
		mesh->SetAiMesh(aiMesh);
		mesh->LoadVBO(aiMesh);
		mesh->LoadEBO(aiMesh);
		mesh->CreateVAO();
		mesh->SetMaterialIndex(aiMesh->mMaterialIndex);
		meshes.push_back(mesh);
	}
}

void Model::Draw()
{
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		(*it)->Draw(materials);

	App->draw->Draw(App->camera->GetFrustum()->ViewMatrix(), App->camera->GetFrustum()->ProjectionMatrix(), SCREEN_WIDTH, SCREEN_HEIGHT);
}

int Model::GetNumVertices()
{
	int numVertices = 0;
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		numVertices +=(*it)->GetNumVertices();

	return numVertices;
}

int Model::GetNumIndices()
{
	int numIndices = 0;
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		numIndices += (*it)->GetNumIndices();

	return numIndices;
}
