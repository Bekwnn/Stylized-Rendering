#include "AssimpUtil.h"

std::string aiu::Import(aiu::Mesh& mesh, std::string& pfile)
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
	aiNode* curNode = scene->mRootNode;
	while (curNode->mNumMeshes == 0)
	{
		if (curNode->mChildren == NULL)
			break;
		else
			curNode = curNode->mChildren[0];
	}
	if (curNode->mNumMeshes != 0)
	{
		mesh = aiu::Mesh(scene->mMeshes[curNode->mMeshes[0]]);
	}

	// We're done. Everything will be cleaned up by the importer destructor
	return "FBX Scene imported successfully. Mesh: " + std::to_string(mesh.mVertices.size());
}