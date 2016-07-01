#pragma once
#include "MeshActor.h"

class TexturedMesh : public MeshActor
{
public:
	TexturedMesh();
	virtual void GenBuffers() override;
	virtual void SetUniforms() override;
	virtual void SetBufferData() override;
	virtual void CleanUp() override;

	virtual void SetTexture(const char* ifile);

	aiu::Texture* mainTexture;

	GLuint uvBuffer;
	GLuint mainTexID;
};