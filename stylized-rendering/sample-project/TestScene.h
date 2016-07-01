#pragma once
#include "Scene.h"
#include "TexturedMesh.h"

class TestScene : public Scene
{
public:
	TestScene();
	virtual ~TestScene();

	void UpdateGUI() override;
	void UpdateScene() override;

private:
	TexturedMesh* meshActor;
	ImVec4 clear_color;
	float f;
	std::string importSuccess;
};