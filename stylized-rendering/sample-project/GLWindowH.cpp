#include "GLWindowH.h"

GLWindowH::GLWindowH()
{
}

GLWindowH::~GLWindowH()
{
	ReleaseDC(hWnd, hDc);
	wglDeleteContext(glC);
	DestroyWindow(hWnd);
}
