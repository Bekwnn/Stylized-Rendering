#include <GL\glew.h>
#include <GL\GL.h>

#include <imgui.h>
#include <assimp\scene.h>

#include "TestScene.h"
#include "AssimpUtil.h"

TestScene::TestScene() :
	Scene::Scene()
{
	clear_color = ImColor(142, 172, 246);
	f = 0.0f;

	std::string cubeMeshFileStr = "../../meshes/export/Cube.fbx";
	cubeMesh = new MeshActor();
	cubeMesh->scene = this;
	cubeMesh->SetMesh(cubeMeshFileStr);
	cubeMesh->castsShadow = true;
	cubeMesh->SetShadowShader("Shaders/shadowmap.vs", "Shaders/shadowmap.fs");
	cubeMesh->SetShader("Shaders/basic.vs", "Shaders/basic.fs");
	cubeMesh->GenBuffers();
	sceneActors.push_back(std::unique_ptr<Actor>(cubeMesh));


	std::string beachMeshFileStr = "../../meshes/export/BeachSand.fbx";
	beachMesh = new TexturedMesh();
	beachMesh->scene = this;
	importSuccess = beachMesh->SetMesh(beachMeshFileStr);
	beachMesh->isShadowed = true;
	beachMesh->castsShadow = true;
	beachMesh->SetShadowShader("Shaders/shadowmap.vs", "Shaders/shadowmap.fs");
	beachMesh->SetShader("Shaders/textured.vs", "Shaders/textured.fs");
	beachMesh->SetTexture("../../textures/Sand_3_Diffuse.png");
	beachMesh->SetNormal("../../textures/Sand_3_Normal.png");
	beachMesh->GenBuffers();
	sceneActors.push_back(std::unique_ptr<Actor>(beachMesh));

	std::string oceanMeshFileStr = "../../meshes/export/OceanGrid.fbx";
	oceanMesh = new OceanMesh();
	oceanMesh->scene = this;
	oceanMesh->castsShadow = true;
	oceanMesh->SetShadowShader("Shaders/shadowmap.vs", "Shaders/shadowmap.fs");
	importSuccess = oceanMesh->SetMesh(oceanMeshFileStr);
	oceanMesh->SetShader("Shaders/ocean.vs", "Shaders/ocean.fs");
	oceanMesh->SetOceanDepthTexture("../../textures/OceanDepthMap.png");
	oceanMesh->SetFlowMapTexture("../../textures/OceanFlowMap.png");
	oceanMesh->SetNormalMapTexture("../../textures/OceanNormal.png");
	oceanMesh->SetNormalMapAltTexture("../../textures/OceanNormal2.png");
	oceanMesh->SetPerlinTexture("../../textures/PerlinNoise.png");
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
	ImGui::ColorEdit3("Sky", (float*)&oceanMesh->skyColor[0]);
	ImGui::ColorEdit3("Wave Crest", (float*)&oceanMesh->waveCrest[0]);
	ImGui::ColorEdit3("Wave Valley", (float*)&oceanMesh->waveValley[0]);
	ImGui::DragFloat("Wave Height", &oceanMesh->waveHeight, 0.05f, 0.0f, 2.0f);
	ImGui::DragFloat3("Box Position", &cubeMesh->position[0], 0.5f, -10.f, 10.f);
	ImGui::DragFloat3("Light Position", &light[0], 0.05f, -1.025f, 1.025f);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(oceanMesh->skyColor.x, oceanMesh->skyColor.y, oceanMesh->skyColor.z, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Scene::UpdateScene();
}
