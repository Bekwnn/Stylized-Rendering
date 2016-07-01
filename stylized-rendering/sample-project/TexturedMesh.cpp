#include "TexturedMesh.h"

TexturedMesh::TexturedMesh() :
	MeshActor::MeshActor()
{
}

void TexturedMesh::GenBuffers()
{
	MeshActor::GenBuffers();

	glGenBuffers(1, &uvBuffer);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &mainTexID);
}

void TexturedMesh::SetUniforms()
{
	MeshActor::SetUniforms();

	GLuint mainTexLocation = glGetUniformLocation(shaderProgram, "mainTex");
	if (mainTexLocation != -1)
	{
		glUniform1i(mainTexLocation, 0);
	}

	GLuint texWrapLocation = glGetUniformLocation(shaderProgram, "uvMultiplier");
	if (texWrapLocation != -1)
	{
		glUniform1f(texWrapLocation, 2);
	}
}

void TexturedMesh::SetBufferData()
{
	MeshActor::SetBufferData();

	// set uv data
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mUVCoords.size()*sizeof(glm::vec2),
		mesh.mUVCoords.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// set texture data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainTexID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureStorage2D(mainTexID, 1, GL_RGBA8, mainTexture->x, mainTexture->y);
	glTextureSubImage2D(mainTexID, 0, 0, 0, mainTexture->x, mainTexture->y, GL_RGBA, GL_UNSIGNED_BYTE, mainTexture->data);
}

void TexturedMesh::CleanUp()
{
	MeshActor::CleanUp();

	glDisableVertexAttribArray(1);
}

void TexturedMesh::SetTexture(const char * ifile)
{
	mainTexture = new aiu::Texture(ifile);
}
