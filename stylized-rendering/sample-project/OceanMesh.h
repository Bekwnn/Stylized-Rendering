#pragma once
#include "MeshActor.h"

class OceanMesh : public MeshActor
{
public:
	OceanMesh();
	virtual void GenBuffers() override;
	virtual void SetUniforms() override;
	virtual void SetBufferData() override;
	virtual void CleanUp() override;

	virtual void SetOceanDepthTexture(const char* ifile);
	virtual void SetFlowMapTexture(const char * ifile);
	virtual void SetNormalMapTexture(const char * ifile);
	virtual void SetPerlinTexture(const char * ifile);

	aiu::Texture* oceanDepth;
	aiu::Texture* flowMap;
	aiu::Texture* oceanNormalMap;
	aiu::Texture* perlinNoise;

	GLuint uvBuffer;
	GLuint oceanDepthTexID;
	GLuint flowMapTexID;
	GLuint oceanNormalMapID;
	GLuint perlinID;

	float maxDepth;
	float waveHeight;
	glm::vec3 waveCrest;
	glm::vec3 waveValley;
};