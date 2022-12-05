#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleEngine;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleRenderExercise* renderer2 = nullptr;
	ModuleEngine* engine = nullptr;
	ModuleDebugDraw* draw = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleTexture* texture = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
