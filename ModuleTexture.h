#pragma once
#include "Module.h"
#include "Model.h"



#include "SDL.h"
#include "GL/glew.h"
#include "DirectXTex/DirectXTex.h"

class ModuleTexture :   public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool            Start();
	update_status   Update();
	bool            CleanUp();
	GLuint			GetTexture();
	GLuint			Load(const char* file_name);
	Model*			GetModel();

private:
	DirectX::ScratchImage* retImage = new DirectX::ScratchImage;
	GLuint texture;
	Model* model;
};

