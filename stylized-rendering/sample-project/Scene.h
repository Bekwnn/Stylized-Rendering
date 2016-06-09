#pragma once
#include "STime.h" //for convenience

class Scene
{
public:
	virtual void UpdateGUI() = 0;
	virtual void UpdateScene() = 0;
};