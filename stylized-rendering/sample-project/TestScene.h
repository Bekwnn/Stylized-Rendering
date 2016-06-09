#pragma once
#include "Scene.h"
#include <imgui.h>

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void UpdateGUI() override;
	void UpdateScene() override;

private:
	ImVec4 clear_color;
	float f;
};