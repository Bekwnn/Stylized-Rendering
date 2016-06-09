#include <windows.h>

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <assimp\vector3.h>
#include <bgfx\bgfx.h>
#include <imgui.h>

#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include "ApplicationH.h"

#include "ImGuiImpl.h"

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//assimp
	aiVector3D vec(0.0f, 0.0f, 0.0f);
	vec += aiVector3D(1.0f, 1.0f, 1.0f);
	//bgfx
//	bgfx::init();
	//bgfx::shutdown();
	//imgui
	//ImGui::Begin("This creates a UI window.");
	
	ApplicationH* app = ApplicationH::CreateApplicationH(hInstance, WndProc, _T("OpenGL"));

	GLWindowH* mainWindow = app->CreateGLWindowH("Main Window", 1280, 720);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: %s\n", glewGetErrorString(err));
		return 0;
	}

	ShowWindow(mainWindow->hWnd,
		nCmdShow);
	UpdateWindow(mainWindow->hWnd);

	ImGui_ImplGL3_Init(mainWindow);

	static float f = 0.0f;
	ImVec4 clear_color = ImColor(114, 144, 154);

	// Main message loop:
	bool bflicker = false;
	MSG msg;
	GetMessage(&msg, mainWindow->hWnd, 0, 0);
	while (msg.message != WM_QUIT) //add some && bGameIsPlaying bool to cond
	{
		//process win32 event messages
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//render update
		else
		{	
			/*
			if (bflicker)
			{
				glClearColor(0.0, 0.0, 1.0, 1.0);
				bflicker = false;
			}
			else
			{
				glClearColor(0.0, 0.0, 0.8, 1.0);
				bflicker = true;
			}
			*/
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.z);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplGL3_NewFrame();

			ImGui::Text("Hello, world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
			ImGui::Render();
			SwapBuffers(mainWindow->hDc);
		}
	}

	ImGui_ImplGL3_Shutdown();

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
extern LRESULT ImGui_ImplGL3_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplGL3_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}