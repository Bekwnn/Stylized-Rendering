#include "Camera.h"

#include <Windows.h>

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <imgui.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "STime.h"

Camera::Camera()
{
	position = glm::vec3(0, 0, -5);
	rotation = glm::quat(1, 0, 0, 0);
	fovHalf = 45;
	speed = 5;
	nearClip = 1, farClip = 100;

	cameraUp = glm::vec3(0, 1, 0);

	rmbWasDown = false;
}

Camera::~Camera()
{}

void Camera::Tick()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	cameraLookAt = position + GetForwardVec();

	mouseDelta = GetMousePos() - mousePrevious;

	if (GetKeyState(VK_RBUTTON) & 0x8000 && rmbWasDown)
	{
		Movement();
		FPSLook();
	}

	rmbWasDown = false;

	if (GetKeyState(VK_RBUTTON) & 0x8000)
	{
		rmbWasDown = true;
	}

	mousePrevious = GetMousePos();

	UpdateVP();
	glLoadMatrixf(glm::value_ptr(VP));
}

glm::mat4x4 Camera::GetMVP(glm::mat4x4 model)
{
	return projection * view * model;
}

void Camera::UpdateVP()
{
	glGetIntegerv(GL_VIEWPORT, viewport);
	view = glm::lookAt(position, cameraLookAt, cameraUp);
	projection = glm::perspective(fovHalf, (double)viewport[2] / (double)viewport[3], nearClip, farClip);
	VP = projection * view;
}

glm::vec2 Camera::GetMousePos()
{
	POINT pos;
	GetCursorPos(&pos);
	return glm::vec2(pos.x, pos.y);
}

void Camera::Movement()
{
	//WASD + QE movement
	if (GetKeyState(0x57) & 0x8000 && !(GetKeyState(0x53) & 0x8000)) // if w and !s
	{
		SetPosition(position + GetForwardVec() * speed * STime::GetDeltaTime());
	}
	if (GetKeyState(0x53) & 0x8000 && !(GetKeyState(0x57) & 0x8000)) // if s and !w
	{
		SetPosition(position - GetForwardVec() * speed * STime::GetDeltaTime());
	}
	if (GetKeyState(0x44) & 0x8000 && !(GetKeyState(0x41) & 0x8000)) // if d and !a
	{
		SetPosition(position + GetRightVec() * speed * STime::GetDeltaTime());
	}
	if (GetKeyState(0x41) & 0x8000 && !(GetKeyState(0x44) & 0x8000)) // if a and !d
	{
		SetPosition(position - GetRightVec() * speed * STime::GetDeltaTime());
	}
	if (GetKeyState(0x45) & 0x8000 && !(GetKeyState(0x51) & 0x8000)) // if e and !q
	{
		SetPosition(position + GetUpVec() * speed * STime::GetDeltaTime());
	}
	if (GetKeyState(0x51) & 0x8000 && !(GetKeyState(0x45) & 0x8000)) // if q and !e
	{
		SetPosition(position - GetUpVec() * speed * STime::GetDeltaTime());
	}
}

void Camera::FPSLook()
{
	//TODO: fps camera look
}
