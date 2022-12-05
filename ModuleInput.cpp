#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"


#include "SDL/include/SDL.h"
#include  "ImGui/imgui_impl_sdl.h"


ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {      
            case SDL_QUIT:
               return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;

           

            case SDL_MOUSEBUTTONDOWN:
                mouseButtonPress = sdlEvent.button.button;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseButtonPress = 0;
                break;
			case SDL_MOUSEWHEEL:
                if (sdlEvent.wheel.y > 0) // scroll up
                {
                    zoomVel = 4.0f;
                }
                else if (sdlEvent.wheel.y < 0) // scroll down
                {
                    zoomVel = -4.0f;
                }
				break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_ESCAPE]) {
        return UPDATE_STOP;
    }

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

SDL_Event ModuleInput::GetSDLEvent()
{
    return sdlEvent;
}

float ModuleInput::GetZoomVel()
{
	return zoomVel;
}

void ModuleInput::SetZoomVel(float zoomVelo)
{
    zoomVel = zoomVelo;
}

void ModuleInput::SetMouseButtonPress(int mouseButton)
{
    mouseButtonPress = mouseButton;
}

int ModuleInput::GetMouseButtonPress()
{
    return mouseButtonPress;
}

