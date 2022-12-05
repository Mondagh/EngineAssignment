#pragma once
#include "Module.h"
#include "../Globals.h"

#include "../SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();
	SDL_Event GetSDLEvent();
	float GetZoomVel();
	void  SetZoomVel(float zoomVelo);
	int GetMouseButtonPress();
	void SetMouseButtonPress(int mouseButton);

private:
	const Uint8 *keyboard = NULL;
	SDL_Event sdlEvent;
	float zoomVel;
	int mouseButtonPress;
};