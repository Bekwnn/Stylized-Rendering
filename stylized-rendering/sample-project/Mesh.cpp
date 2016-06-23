#include "AssimpUtil.h"

#include <glm\gtc\type_ptr.hpp>

using namespace aiu;

Mesh::Mesh()
{
	mVertices = std::vector<glm::vec3>();
	mUVCoords = std::vector<glm::vec2>();
	mFaces = std::vector<glm::ivec3>();
}

Mesh::Mesh(aiMesh* aimesh)
{
	//PER VERT
	int vertCount = aimesh->mNumVertices;
	mVertices = std::vector<glm::vec3>(vertCount);
	mUVCoords = std::vector<glm::vec2>(vertCount);
	for (int vertIdx = 0; vertIdx < vertCount; vertIdx++)
	{
		mVertices[vertIdx] = glm::make_vec3(&aimesh->mVertices[vertIdx][0]);
		//mUVCoords[vertIdx] = glm::make_vec2(&aimesh->mTextureCoords[0][vertIdx][0]);
	}
	//PER FACE
	int faceCount = aimesh->mNumFaces;
	mFaces = std::vector<glm::ivec3>(faceCount);
	for (int faceIdx = 0; faceIdx < faceCount; faceIdx++)
	{
		mFaces[faceIdx] = glm::make_vec3(aimesh->mFaces[faceIdx].mIndices);
	}
}