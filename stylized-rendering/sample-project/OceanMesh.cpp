#include "OceanMesh.h"
#include "STime.h"

OceanMesh::OceanMesh() :
	MeshActor::MeshActor()
{
	maxDepth = 30.f;
	waveHeight = 0.1f;
	waveCrest = glm::vec3(5, 100, 170);
	waveValley = glm::vec3(8, 52, 140);
	waveCrest *= 1.f / 255.f;
	waveValley *= 1.f / 255.f;
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
	glTextureStorage2D(flowMapTexID, 1, GL_RGBA8, flowMap->x, flowMap->y);
	glTextureSubImage2D(flowMapTexID, 0, 0, 0, flowMap->x, flowMap->y, GL_RGBA, GL_UNSIGNED_BYTE, flowMap->data);

	// OCEAN NORMAL MAP
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &oceanNormalMapID);
	glBindTexture(GL_TEXTURE_2D, oceanNormalMapID);
	glTextureStorage2D(oceanNormalMapID, 1, GL_RGBA8_SNORM, oceanNormalMap->x, oceanNormalMap->y);
	glTextureSubImage2D(oceanNormalMapID, 0, 0, 0, oceanNormalMap->x, oceanNormalMap->y, GL_RGBA, GL_UNSIGNED_BYTE, oceanNormalMap->data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// PERLIN NOISE
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &perlinID);
	glBindTexture(GL_TEXTURE_2D, perlinID);
	glTextureStorage2D(perlinID, 1, GL_RGBA8, perlinNoise->x, perlinNoise->y);
	glTextureSubImage2D(perlinID, 0, 0, 0, perlinNoise->x, perlinNoise->y, GL_RGBA, GL_UNSIGNED_BYTE, perlinNoise->data);
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

	GLint normalMapLocation = glGetUniformLocation(shaderProgram, "waveNormal");
	if (normalMapLocation != -1)
	{
		glUniform1i(normalMapLocation, 2);
	}

	GLint perlinLocation = glGetUniformLocation(shaderProgram, "perlin");
	if (perlinLocation != -1)
	{
		glUniform1i(perlinLocation, 3);
	}

	GLint waveHeightLocation = glGetUniformLocation(shaderProgram, "waveHeight");
	if (waveHeightLocation != -1)
	{
		glUniform1f(waveHeightLocation, waveHeight);
	}

	GLint timeLocation = glGetUniformLocation(shaderProgram, "time");
	if (timeLocation != -1)
	{
		glUniform1f(timeLocation, STime::GetCurTime());
	}

	GLint crestLocation = glGetUniformLocation(shaderProgram, "waveCrestColor");
	if (crestLocation != -1)
	{
		glUniform3fv(crestLocation, 1, &waveCrest[0]);
	}

	GLint valleyLocation = glGetUniformLocation(shaderProgram, "waveValleyColor");
	if (valleyLocation != -1)
	{
		glUniform3fv(valleyLocation, 1, &waveValley[0]);
	}
}

void OceanMesh::SetBufferData()
{
	MeshActor::SetBufferData();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, oceanDepthTexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, flowMapTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, oceanNormalMapID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, perlinID);

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

void OceanMesh::SetNormalMapTexture(const char * ifile)
{
	oceanNormalMap = new aiu::Texture(ifile);
}

void OceanMesh::SetPerlinTexture(const char * ifile)
{
	perlinNoise = new aiu::Texture(ifile);
}