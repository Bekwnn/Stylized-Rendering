#pragma once
#include "Scene.h"
#include "MeshActor.h"

class TestScene : public Scene
{
public:
	TestScene();
	virtual ~TestScene();

	void UpdateGUI() override;
	void UpdateScene() override;

private:
	MeshActor* meshActor;
	ImVec4 clear_color;
	float f;
	std::string importSuccess;
};