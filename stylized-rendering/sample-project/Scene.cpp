#include "Scene.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

std::string Scene::Import(std::string& pfile)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pfile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		return importer.GetErrorString();
	}

	// Now we can access the file's contents.
	// TODO: access the file's meshes and return them

	// We're done. Everything will be cleaned up by the importer destructor
	return "Blender scene imported successfully.";
}