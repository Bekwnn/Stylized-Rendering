#include "OceanMesh.h"

OceanMesh::OceanMesh() :
	MeshActor::MeshActor()
{
	maxDepth = 30.f;
}

void OceanMesh::GenBuffers()
{
	MeshActor::GenBuffers();

	glGenBuffers(1, &uvBuffer);

	glEnable(GL_TEXTURE_2D);

	// OCEAN DEPTH HEIGHTMAP
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &oceanDepthTexID);
	glBindTexture(GL_TEXTURE_2D, oceanDepthTexID);
	glTextureStorage2D(oceanDepthTexID, 1, GL_RGBA8, oceanDepth->x, oceanDepth->y);
	glTextureSubImage2D(oceanDepthTexID, 0, 0, 0, oceanDepth->x, oceanDepth->y, GL_RGBA, GL_UNSIGNED_BYTE, oceanDepth->data);

	// WATER FLOW MAP
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &flowMapTexID);
	glBindTexture(GL_TEXTURE_2D, flowMapTexID);
	glTextureStorage2D(flowMapTexID, 1, GL_RGBA8_SNORM, flowMap->x, flowMap->y);
	glTextureSubImage2D(flowMapTexID, 0, 0, 0, flowMap->x, flowMap->y, GL_RGBA, GL_UNSIGNED_BYTE, flowMap->data);
}

void OceanMesh::SetUniforms()
{
	MeshActor::SetUniforms();

	GLint oceanDepthTexLocation = glGetUniformLocation(shaderProgram, "oceanDepthMap");
	if (oceanDepthTexLocation != -1)
	{
		glUniform1i(oceanDepthTexLocation, 0);
	}

	GLint flowMapTexLocation = glGetUniformLocation(shaderProgram, "flowMap");
	if (flowMapTexLocation != -1)
	{
		glUniform1i(flowMapTexLocation, 1);
	}

	GLint waveHeightLocation = glGetUniformLocation(shaderProgram, "waveHeight");
	if (waveHeightLocation != -1)
	{
		glUniform1f(waveHeightLocation, waveHeight);
	}

	GLint choppinessLocation = glGetUniformLocation(shaderProgram, "choppiness");
	if (choppinessLocation != -1)
	{
		glUniform1f(choppinessLocation, choppiness);
	}
}

void OceanMesh::SetBufferData()
{
	MeshActor::SetBufferData();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, oceanDepthTexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, flowMapTexID);

	// set uv data
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mUVCoords.size()*sizeof(glm::vec2),
		mesh.mUVCoords.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void OceanMesh::CleanUp()
{
	MeshActor::CleanUp();

	glDisableVertexAttribArray(4);
}

void OceanMesh::SetOceanDepthTexture(const char * ifile)
{
	oceanDepth = new aiu::Texture(ifile);
}

void OceanMesh::SetFlowMapTexture(const char * ifile)
{
	flowMap = new aiu::Texture(ifile);
}
