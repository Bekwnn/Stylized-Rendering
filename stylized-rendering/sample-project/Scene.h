/// an abstract class which defines the interface of a scene

#pragma once
#include "STime.h" //for convenience
#include <string>

class Scene
{
public:
	virtual void UpdateGUI() = 0;
	virtual void UpdateScene() = 0;

	//TODO: move elsewhere
	std::string Import(std::string& pfile);

	std::vector<Actor> sceneActors;
};