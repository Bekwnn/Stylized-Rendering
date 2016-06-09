#include <GL\glew.h>
#include <GL\GL.h>

#include "TestScene.h"

TestScene::TestScene()
{
	clear_color = ImColor(114, 144, 154);
	f = 0.0f;
}

TestScene::~TestScene()
{
}

void TestScene::UpdateGUI()
{
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CurrentTime: %.3f seconds", STime::GetCurTime());
}

void TestScene::UpdateScene()
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}
