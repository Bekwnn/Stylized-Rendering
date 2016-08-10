#pragma once
#include "TexturedMesh.h"

class BushMesh : public TexturedMesh
{
public:
	BushMesh();
	virtual void ShadowPass() override;
	virtual void GenBuffers() override;
	virtual void SetUniforms() override;
	virtual void SetBufferData() override;

	void SetStiffMap(const char* ifile);

	aiu::Texture* stiffnessMap;

	GLuint stiffMapID;

	glm::vec3 wind;
};