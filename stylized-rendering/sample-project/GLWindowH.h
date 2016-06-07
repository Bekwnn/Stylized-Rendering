//wrapper for the WINAPI window

#pragma once
#include <windows.h>
#include <string>

class GLWindowH
{
public:
	GLWindowH();
	~GLWindowH();


	HWND hWnd; //WINAPI handle for the window
	HDC hDc; //WINAPI handle for device context
	HGLRC glC; //WINAPI handle for opengl context

};