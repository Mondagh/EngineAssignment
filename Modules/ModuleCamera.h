#pragma once
#include "Module.h"
#include "Geometry/Frustum.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool            Start();
	update_status   Update();
	bool            CleanUp();
	void SetFOV(float degree, float aspectRadio);
	void SetAspectRatio(float vFOV, float aspectRadio);
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPos(const vec& pos);
	void SetUp(const vec& up);
	void SetFront(const vec& front);
	void Translate(const vec& tranlate);
	void Rotate(const float3x3& rotationMatrix);
	void LookAt(float x, float y, float z);
	void Zoom(float zoom);
	void Focus();

	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();
	Frustum* GetFrustum();
private:
	Frustum frustum;
	float velocity = 0.4f;

};

