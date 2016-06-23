#include "Scene.h"

#include <assimp\scene.h>
#include <assimp\postprocess.h>

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


