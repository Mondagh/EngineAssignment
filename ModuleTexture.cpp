#include "ModuleTexture.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"

#include "DirectXTex/DirectXTex.h"
#include "SDL.h"
#include "GL/glew.h"

#include <string>
#include <codecvt>
#include <locale>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Start()
{
	const char* path2 = "assets/BakerHouse.fbx";

	model = new Model();
	model->Load(path2);
	return true;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	glDeleteTextures(1,&texture);
	return false;
}

GLuint ModuleTexture::GetTexture()
{
	return texture;
}

GLuint ModuleTexture::Load(const char* file_name)
{
	GLuint texture;
	HRESULT loadResult;
	DirectX::TexMetadata info;
	string fileName = "assets/";
	string fileNameS = file_name;
	fileName.append(fileNameS);
	using convert_t = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_t, wchar_t> strconverter;
	std::wstring widePath = strconverter.from_bytes(fileName);

	loadResult = LoadFromWICFile(widePath.c_str(), DirectX::WIC_FLAGS_NONE, &info, *retImage);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLint internalFormat = GL_RGBA8;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;

	switch (retImage->GetMetadata().format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		break;
	}

	DirectX::ScratchImage image;
	DirectX::FlipRotate(retImage->GetImages(), retImage->GetImageCount(), retImage->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, image);

	const DirectX::Image* imatge = image.GetImage(0, 0, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imatge->width, imatge->height, 0, format, type, imatge->pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

Model* ModuleTexture::GetModel()
{
	return model;
}
