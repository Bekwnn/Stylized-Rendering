#include "TexturedMesh.h"

#include <glm\gtc\type_ptr.hpp>

#include "Scene.h"

TexturedMesh::TexturedMesh() :
	MeshActor::MeshActor()
{
	specPow = 32.f;
	specMul = 0.2f;
	normalMapped = false;
}

void TexturedMesh::GenBuffers()
{
	MeshActor::GenBuffers();

	glGenBuffers(1, &uvBuffer);

	glEnable(GL_TEXTURE_2D);

	// DIFFUSE TEXTURE
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &mainTexID);
	glBindTexture(GL_TEXTURE_2D, mainTexID);
	glTextureStorage2D(mainTexID, 1, GL_RGBA8, mainTexture->x, mainTexture->y);
	glTextureSubImage2D(mainTexID, 0, 0, 0, mainTexture->x, mainTexture->y, GL_RGBA, GL_UNSIGNED_BYTE, mainTexture->data);

	float anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	// NORMAL MAP
	if (normalMapped)
	{
		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1, &normalTexID);
		glBindTexture(GL_TEXTURE_2D, normalTexID);
		glTextureStorage2D(normalTexID, 1, GL_RGBA8_SNORM, normalTexture->x, normalTexture->y);
		glTextureSubImage2D(normalTexID, 0, 0, 0, normalTexture->x, normalTexture->y, GL_RGBA, GL_UNSIGNED_BYTE, normalTexture->data);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void TexturedMesh::SetUniforms()
{
	MeshActor::SetUniforms();

	GLint mainTexLocation = glGetUniformLocation(shaderProgram, "diffuseMap");
	if (mainTexLocation != -1)
	{
		glUniform1i(mainTexLocation, 0);
	}

	GLint normTexLocation = glGetUniformLocation(shaderProgram, "normalMap");
	if (normalMapped && normTexLocation != -1)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "useNormalMapping"), 1);
		glUniform1i(normTexLocation, 1);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "useNormalMapping"), 0);
	}

	GLint texWrapLocation = glGetUniformLocation(shaderProgram, "uvMultiplier");
	if (texWrapLocation != -1)
	{
		glUniform1f(texWrapLocation, 8);
	}

	GLint specMulLocation = glGetUniformLocation(shaderProgram, "specMultiplier");
	if (specMulLocation != -1)
	{
		glUniform1f(specMulLocation, specMul);
	}

	GLint specPowLocation = glGetUniformLocation(shaderProgram, "specPower");
	if (specPowLocation != -1)
	{
		glUniform1f(specPowLocation, specPow);
	}
}

void TexturedMesh::SetBufferData()
{
	MeshActor::SetBufferData();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainTexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexID);

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

void TexturedMesh::CleanUp()
{
	MeshActor::CleanUp();

	glDisableVertexAttribArray(4);
}

void TexturedMesh::SetTexture(const char * ifile)
{
	mainTexture = new aiu::Texture(ifile);
}

void TexturedMesh::SetNormal(const char * ifile)
{
	normalTexture = new aiu::Texture(ifile);
}
