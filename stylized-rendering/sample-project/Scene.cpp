#include "Scene.h"

#include <assimp\scene.h>
#include <assimp\postprocess.h>

Scene::Scene()
{
	light = glm::vec3(0.f, 20.f, 0.f);
}

void Scene::UpdateScene()
{
	for (auto& actor : sceneActors)
	{
		actor->Tick();
	}

	camera->Tick();

	for (auto& actor : sceneActors)
	{
		actor->Render();
	}
}


