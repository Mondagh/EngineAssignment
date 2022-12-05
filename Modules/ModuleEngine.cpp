#include "ModuleEngine.h"
#include "ModuleWindow.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <stdio.h>
#include <SDL.h>
#include "Math/float3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


using namespace std;

ModuleEngine::ModuleEngine()
{
}

ModuleEngine::~ModuleEngine()
{
}

bool ModuleEngine::Init()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_MakeCurrent(App->window->window, App->renderer->context);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		io.ConfigViewportsNoAutoMerge = false;
		io.ConfigViewportsNoTaskBarIcon = true;
	}

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		io.ConfigDockingTransparentPayload = true;
	}
	return true;
}

bool ModuleEngine::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.10f));
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor(0.54f, 0.43f, 0.12f, 0.97f));


	return true;
}

update_status ModuleEngine::PreUpdate()
{
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	

	return UPDATE_CONTINUE;
}

update_status ModuleEngine::Update()
{


	static float f = 0.0f;
	static int counter = 0;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static bool showLibraries = true;

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Menu")) {
		if (ImGui::MenuItem("Quit")) {
			return UPDATE_STOP;
		}
		if (ImGui::MenuItem("Github")) {
			ShellExecuteA(NULL, "open", "https://github.com/Mondagh/EngineAssignment", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::BeginMenu("About")) {
			ImGui::Text("Engine in development");
			ImGui::Separator();
			ImGui::Text("Libraries");
			ImGui::Separator();
			ImGui::Text("OpenGL 4.6");
			ImGui::Text("SDL 2.26.1");
			ImGui::Text("Assimp 5.2.5");
			ImGui::Text("DirectTex");
			ImGui::Text("Glew 2.1.0");
			ImGui::Text("MathGeoLib");
			ImGui::Text("ImGui 1.89.1");
			ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_FirstUseEver);
			ImGui::EndMenu();
		}		
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Configuration")) {
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::EndMenu();
	}

	

	ImGui::EndMainMenuBar();










	ImGui::Begin("GameObject");


	float3 pos = App->camera->GetFrustum()->Pos();

	ImGui::Text("Transformation (X,Y,Z)");
	ImGui::InputFloat3("Position", pos.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);

	ImGui::Separator();

	float3 front = App->camera->GetFrustum()->Front();
	float3 up = App->camera->GetFrustum()->Up();
	float nearPlane = App->camera->GetFrustum()->NearPlaneDistance();
	float farPlane = App->camera->GetFrustum()->FarPlaneDistance();
	float verticalFov = App->camera->GetFrustum()->VerticalFov();


	ImGui::Text("Frustum");
	ImGui::InputFloat3("Front", front.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("Up", up.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("Near Plane", &nearPlane, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("Far Plane", &farPlane, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("FOV", &verticalFov, "%.3f", ImGuiInputTextFlags_ReadOnly);


	ImGui::Separator();

	
	int numVertices = App->texture->GetModel()->GetNumVertices();
	int numIndices = App->texture->GetModel()->GetNumIndices();
		
	ImGui::Text("Texture");
	ImGui::TextWrapped("Num Vertices: ");
	ImGui::SameLine();
	ImGui::Text("%d", numVertices);
	ImGui::TextWrapped("Num Triangles: ");
	ImGui::SameLine();
	ImGui::Text( "%d", numIndices / 3);

	ImGui::End();



























	return UPDATE_CONTINUE;
}

update_status ModuleEngine::PostUpdate()
{


	ImGuiIO& io = ImGui::GetIO(); (void)io;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);

	

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->renderer->context);

	return UPDATE_CONTINUE;
}


bool ModuleEngine::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return false;
}
