// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <stdio.h>
#include "ApplicationH.h"

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	ApplicationH* app = ApplicationH::CreateApplicationH(hInstance, WndProc, _T("OpenGL"));

	GLWindowH* mainWindow = app->CreateGLWindowH("Main Window", 1280, 720);

	ShowWindow(mainWindow->hWnd,
		nCmdShow);
	UpdateWindow(mainWindow->hWnd);

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
			glClear(GL_COLOR_BUFFER_BIT);

			glLoadIdentity();

			glFlush();

			SwapBuffers(mainWindow->hDc);
		}
	}

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}