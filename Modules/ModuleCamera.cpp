#include "../Globals.h"
#include "ModuleCamera.h"
#include "../Application.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"



#include "GL/glew.h"
#include "Geometry/Frustum.h"
#include "Math/float3x3.h"
#include "Math/float3.h"
#include "../SDL/include/SDL.h"


typedef unsigned __int8 Uint8;

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Start()
{
	
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);


	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(math::pi / 4.0f, aspect);
	frustum.SetFront(-vec::unitZ);
	frustum.SetUp(vec::unitY);
	SetPos(float3(1.0f, 1.0f, 8.0f));


	LookAt(0, 0, 0);
	
	return true;
}

update_status ModuleCamera::Update()
{
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	SDL_Event sdlEvent = App->input->GetSDLEvent();

	int mouseButtonPress = App->input->GetMouseButtonPress();
	float zoomVel = App->input->GetZoomVel();
	float camaraVel = velocity;
	if (zoomVel != 0) {
		Zoom(zoomVel);
		App->input->SetZoomVel(0.0f);
	}

	if (keyboard[SDL_SCANCODE_LSHIFT]) {
		camaraVel = velocity * 2.0f;
	}
	if (mouseButtonPress == SDL_BUTTON_LEFT)
	{
		if (keyboard[SDL_SCANCODE_LALT]) {

			vec oldFocus = GetFrustum()->Pos() + GetFrustum()->Front().Normalized() * GetFrustum()->Pos().Length();
			float4x4 model = App->program->model;
			Rotate(float3x3::RotateAxisAngle(GetFrustum()->WorldRight().Normalized(), -model.y * 0.1));
			Rotate(float3x3::RotateY(-model.x * 0.1));
			vec newFocus = GetFrustum()->Pos() + GetFrustum()->Front().Normalized() * GetFrustum()->Pos().Length();
			Translate(oldFocus - newFocus);
		}
	}

	if (mouseButtonPress == SDL_BUTTON_RIGHT)
	{
		if (keyboard[SDL_SCANCODE_A]) {
			const vec& tranlate = GetFrustum()->WorldRight().Normalized() * -camaraVel;
			Translate(tranlate);
		}
		if (keyboard[SDL_SCANCODE_D]) {
			const vec& tranlate = GetFrustum()->WorldRight().Normalized() * camaraVel;
			Translate(tranlate);
		}
		if (keyboard[SDL_SCANCODE_S]) {
			const vec& tranlate = GetFrustum()->Front().Normalized() * -camaraVel;
			Translate(tranlate);
		}
		if (keyboard[SDL_SCANCODE_W]) {
			const vec& tranlate = GetFrustum()->Front().Normalized() * camaraVel;
			Translate(tranlate);
		}
	}
	if (keyboard[SDL_SCANCODE_Q]) {
		const vec& tranlate = GetFrustum()->Up().Normalized() * -camaraVel;
		Translate(tranlate);
	}
	if (keyboard[SDL_SCANCODE_E]) {
		const vec& tranlate = GetFrustum()->Up().Normalized() * camaraVel;
		Translate(tranlate);
	}
	if (keyboard[SDL_SCANCODE_F]) {
		Focus();
	}
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return false;
}

void ModuleCamera::SetFOV(float degree, float aspectRadio)
{
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(degree), aspectRadio);
}

void ModuleCamera::SetAspectRatio(float vFOV,float aspectRadio)
{
	frustum.SetVerticalFovAndAspectRatio(vFOV, aspectRadio);
}

void ModuleCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
}

void ModuleCamera::SetPos(const vec& pos)
{
	frustum.SetPos(pos);
}

void ModuleCamera::SetUp(const vec& up)
{
	frustum.SetUp(up);
}

void ModuleCamera::SetFront(const vec& front)
{
	frustum.SetFront(front);
}

void ModuleCamera::Translate(const vec& tranlate)
{
	GetFrustum()->SetPos(GetFrustum()->Pos() + tranlate);
}

void ModuleCamera::Rotate(const float3x3& rotationMatrix)
{
	Frustum* frustum = GetFrustum();
	vec oldFront = frustum->Front().Normalized();
	vec oldUp = frustum->Up().Normalized();
	frustum->SetFront(rotationMatrix * oldFront);
	frustum->SetUp(rotationMatrix * oldUp);
}

void ModuleCamera::LookAt(float x, float y, float z)
{
	Frustum* frustum = GetFrustum();
	vec direction = vec(x, y, z) - frustum->Pos();
	direction.Normalize();
	vec up = vec::unitY;

	Rotate(float3x3::LookAt(frustum->Front().Normalized(), direction, frustum->Up().Normalized(), up));
}

void ModuleCamera::Zoom(float zoom)
{
	Translate(GetFrustum()->Front().Normalized() * zoom);
}

void ModuleCamera::Focus()
{
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	GetFrustum()->SetViewPlaneDistances(0.1f, 100.0f);
	GetFrustum()->SetHorizontalFovAndAspectRatio(math::pi / 4.0f, aspect);
	GetFrustum()->SetFront(-vec::unitZ);
	GetFrustum()->SetUp(vec::unitY);
	SetPos(float3(1.0f, 1.0f, 8.0f));
	LookAt(0, 0, 0);
}




float4x4 ModuleCamera::GetProjectionMatrix()
{
	Frustum frustum;
	GetFrustum()->SetKind(FrustumSpaceGL, FrustumRightHanded);
	GetFrustum()->SetViewPlaneDistances(0.1f, 200.0f);
	GetFrustum()->SetHorizontalFovAndAspectRatio(DegToRad(90.0f), 1.3f);
	GetFrustum()->SetPos(float3(0.0f, 1.0f, -2.0f));
	GetFrustum()->SetFront(float3::unitZ);
	GetFrustum()->SetUp(float3::unitY);
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	//Send the frustum projection matrix to OpenGL
	// direct mode would be:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);

	return projectionGL;
}

float4x4 ModuleCamera::GetViewMatrix()
{
	GetFrustum()->SetPos(float3::zero);
	float3x3 rotationMatrix; // = some rotation value (or LookAt matrix)
	GetFrustum()->SetFront(rotationMatrix.WorldX());
	GetFrustum()->SetUp(rotationMatrix.WorldY());
	//Send the frustum view matrix to OpenGL
	// direct mode would be:
	float4x4 viewGL = float4x4(GetFrustum()->ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);

	return viewGL;
}

Frustum* ModuleCamera::GetFrustum()
{
	return &frustum;
}
