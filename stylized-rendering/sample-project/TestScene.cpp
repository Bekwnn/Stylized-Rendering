#include <GL\glew.h>
#include <GL\GL.h>

#include <imgui.h>
#include <assimp\scene.h>

#include "TestScene.h"
#include "AssimpUtil.h"

TestScene::TestScene()
{
	clear_color = ImColor(142, 172, 246);
	f = 0.0f;

	std::string beachMeshFileStr = "../../meshes/export/BeachSand.fbx";
	beachMesh = new TexturedMesh();
	beachMesh->scene = this;
	importSuccess = beachMesh->SetMesh(beachMeshFileStr);
	beachMesh->SetShader("texvert.vs", "texfrag.fs");
	beachMesh->SetTexture("../../textures/Sand_3_Diffuse.png");
	beachMesh->SetNormal("../../textures/Sand_3_Normal.png");
	beachMesh->GenBuffers();
	sceneActors.push_back(std::unique_ptr<Actor>(beachMesh));

	std::string oceanMeshFileStr = "../../meshes/export/OceanGrid.fbx";
	oceanMesh = new OceanMesh();
	oceanMesh->scene = this;
	importSuccess = oceanMesh->SetMesh(oceanMeshFileStr);
	oceanMesh->SetShader("oceanvert.vs", "oceanfrag.fs");
	oceanMesh->SetOceanDepthTexture("../../textures/OceanDepthMap.png");
	oceanMesh->SetFlowMapTexture("../../textures/OceanFlowMap.png");
	oceanMesh->GenBuffers();
	sceneActors.push_back(std::unique_ptr<Actor>(oceanMesh));

	camera = std::unique_ptr<Camera>(new Camera());
}

TestScene::~TestScene() {}

void TestScene::UpdateGUI()
{
	ImGui::Begin("Debug");
	aiu::Mesh* mesh = &beachMesh->mesh;
	ImGui::Text(importSuccess.c_str());
	ImGui::Text("Mesh Vertices: %d", mesh->mVertices.size());
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	ImGui::DragFloat("Spec Mul", &beachMesh->specMul, 0.05f, 0.f, 0.5f);
	ImGui::DragFloat("Spec Pow", &beachMesh->specPow, 0.5f, 0.f, 50.f);
	ImGui::Checkbox("Normal Map", &beachMesh->normalMapped);
	ImGui::DragFloat3("Light Position", &beachMesh->lightPosition[0], 10.f, -1000.f, 1000.f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CurrentTime: %.3f seconds", STime::GetCurTime());
	ImGui::Checkbox("RMB Down", &(camera->rmbWasDown));
	ImGui::Text("Camera Position: (%f, %f, %f)", camera->position.x, camera->position.y, camera->position.z);
	glm::vec3 lookdir = camera->GetForwardVec();
	ImGui::Text("Camera Look Dir: (%f, %f, %f)", lookdir.x, lookdir.y, lookdir.z);
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
