#include <GL\glew.h>
#include <GL\GL.h>

#include <imgui.h>

#include "TestScene.h"

TestScene::TestScene()
{
	clear_color = ImColor(114, 144, 154);
	f = 0.0f;
	std::string blendFileStr = "../../../../BlenderCreated/skeletree.blend";
	importSuccess = Import(blendFileStr);
}

TestScene::~TestScene() {}

void TestScene::UpdateGUI()
{
	ImGui::Text(importSuccess.c_str());
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CurrentTime: %.3f seconds", STime::GetCurTime());
	ImGui::Checkbox("RMB Down", &(camera.rmbWasDown));
	ImGui::Text("Camera Position: (%f, %f, %f)", camera.position.x, camera.position.y, camera.position.z);
	glm::vec3 lookdir = camera.GetForwardVec();
	ImGui::Text("Camera Look Dir: (%f, %f, %f)", lookdir.x, lookdir.y, lookdir.z);
}

void TestScene::UpdateScene()
{
	Scene::UpdateScene();

	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}
