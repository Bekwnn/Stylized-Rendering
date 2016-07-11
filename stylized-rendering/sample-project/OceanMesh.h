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

	aiu::Texture* oceanDepth;
	aiu::Texture* flowMap;

	GLuint uvBuffer;
	GLuint oceanDepthTexID;
	GLuint flowMapTexID;

	float maxDepth;
	float waveHeight;
	float choppiness;
};