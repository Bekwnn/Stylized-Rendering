#include <GL\glew.h>
#include <GL\GL.h>

#include <imgui.h>
#include <assimp\scene.h>

#include "TestScene.h"
#include "TestActor.h"
#include "AssimpUtil.h"

TestScene::TestScene()
{
	clear_color = ImColor(114, 144, 154);
	f = 0.0f;
	std::string meshFileStr = "../../meshes/export/BeachSand.fbx";
	meshActor = new MeshActor();
	meshActor->scene = this;
	importSuccess = meshActor->SetMesh(meshFileStr);
	meshActor->SetShader("vert.vs", "frag.fs");
	sceneActors.push_back(std::unique_ptr<Actor>(meshActor));
	camera = std::unique_ptr<Camera>(new Camera());
}

TestScene::~TestScene() {}

void TestScene::UpdateGUI()
{
	ImGui::Begin("Debug");
	aiu::Mesh* mesh = &meshActor->mesh;
	ImGui::Text(importSuccess.c_str());
	ImGui::Text("Mesh Vertices: %d", mesh->mVertices.size());
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CurrentTime: %.3f seconds", STime::GetCurTime());
	ImGui::Checkbox("RMB Down", &(camera->rmbWasDown));
	ImGui::Text("Camera Position: (%f, %f, %f)", camera->position.x, camera->position.y, camera->position.z);
	glm::vec3 lookdir = camera->GetForwardVec();
	ImGui::Text("Camera Look Dir: (%f, %f, %f)", lookdir.x, lookdir.y, lookdir.z);
	for (int i = 0; i < 50; i++)
	{
		ImGui::Text("f%d: (%d, %d, %d)", i, mesh->mFaces[i].x, mesh->mFaces[i].y, mesh->mFaces[i].z);
	}
	ImGui::End();
}

void TestScene::UpdateScene()
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::UpdateScene();
}
