#pragma once
#include "Scene.h"
#include "TexturedMesh.h"
#include "OceanMesh.h"

class TestScene : public Scene
{
public:
	TestScene();
	virtual ~TestScene();

	void UpdateGUI() override;
	void UpdateScene() override;

private:
	TexturedMesh* beachMesh;
	OceanMesh* oceanMesh;
	ImVec4 clear_color;
	float f;
	std::string importSuccess;
};