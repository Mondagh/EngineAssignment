#include "Mesh.h"
#include "Application.h"
#include "Modules/ModuleTexture.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleRender.h"
#include "Modules/ModuleRenderExercise.h"
#include "Modules/ModuleProgram.h"


#include <Assimp/mesh.h>
#include <Assimp/scene.h>
#include <Assimp/cimport.h>
#include <Assimp/postprocess.h>
#include "Math/float2.h"

#include <assert.h>  


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	num_vertices = mesh->mNumVertices;

}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	num_indices = mesh->mNumFaces * 3;
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices));
}

void Mesh::Draw(const std::vector<unsigned>& model_textures)
{
	unsigned program = App->renderer2->GetProgram();
	const float4x4& view = App->camera->GetFrustum()->ViewMatrix();
	const float4x4& proj = App->camera->GetFrustum()->ProjectionMatrix();
	float4x4 model = App->program->model = float4x4::float4x4::FromTRS(float3(1.0f, 0.0f, 1.0f), float4x4::identity, float3(1.0f, 1.0f, 1.0f));

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[materialIndex]);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}

aiMesh* Mesh::GetAiMeshe()
{
	return aiMeshe;
}

void Mesh::SetAiMesh(aiMesh* mesh)
{
	aiMeshe = mesh;
}

void Mesh::SetMaterialIndex(const unsigned int numMaterialIndex)
{
	materialIndex = numMaterialIndex;
}

int Mesh::GetNumVertices()
{
	return num_vertices;
}
int Mesh::GetNumIndices()
{
	return num_indices;
}