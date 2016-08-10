#include "BushMesh.h"
#include "Scene.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

BushMesh::BushMesh() :
	TexturedMesh::TexturedMesh()
{
	position = glm::vec3(-0.5f, 1.f, -0.5f);
	scale = glm::vec3(0.2f, 0.2f, 0.2f);
	wind = glm::vec3(1.f, 0.f, 0.f);
	castsShadow = true;
	isShadowed = false;
}

void BushMesh::ShadowPass()
{
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-25, 25, -25, 25, -25, 25);
	glm::mat4 depthViewMatrix = glm::lookAt(scene->light, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = GetModelMatrix();
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// compute depthbiasMVP for actual shading step later
	depthBiasMVP = biasMatrix*depthMVP;

	if (!castsShadow) return;	//hop off here if we don't want this to cast shadows

	glUseProgram(shadowProgram);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set MVP in shadow shader
	GLint depthModelID = glGetUniformLocation(shadowProgram, "depthM");
	if (depthModelID != -1)
	{
		glUniformMatrix4fv(depthModelID, 1, GL_FALSE, &depthModelMatrix[0][0]);
	}

	GLint depthViewID = glGetUniformLocation(shadowProgram, "depthV");
	if (depthViewID != -1)
	{
		glUniformMatrix4fv(depthViewID, 1, GL_FALSE, &depthViewMatrix[0][0]);
	}

	GLint depthProjID = glGetUniformLocation(shadowProgram, "depthP");
	if (depthProjID != -1)
	{
		glUniformMatrix4fv(depthProjID, 1, GL_FALSE, &depthProjectionMatrix[0][0]);
	}

	GLint mainTexLocation = glGetUniformLocation(shadowProgram, "diffuse");
	if (mainTexLocation != -1)
	{
		glUniform1i(mainTexLocation, 0);
	}

	GLint timeLoc = glGetUniformLocation(shadowProgram, "time");
	if (timeLoc != -1)
	{
		glUniform1f(timeLoc, STime::GetCurTime());
	}

	GLint stiffLoc = glGetUniformLocation(shadowProgram, "stiffness");
	if (stiffLoc != -1)
	{
		glUniform1i(stiffLoc, 2);
	}

	GLint windLoc = glGetUniformLocation(shadowProgram, "wind");
	if (windLoc != -1)
	{
		glUniform3fv(windLoc, 1, &wind[0]);
	}

	FreeBuffers();

	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, stiffMapID);

	// set element buffer with face indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		mesh.mFaces.size() * sizeof(glm::ivec3),
		mesh.mFaces.data(),
		GL_STATIC_DRAW
		);

	// set array buffer with vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mVertices.size() * sizeof(glm::vec3),
		mesh.mVertices.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// set uv data
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mUVCoords.size()*sizeof(glm::vec2),
		mesh.mUVCoords.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// draw by index
	glDrawElements(
		GL_TRIANGLES,
		mesh.mFaces.size() * 3,
		GL_UNSIGNED_INT,
		NULL
		);

	glDisableVertexAttribArray(0);

	FreeBuffers();
}

void BushMesh::GenBuffers()
{
	TexturedMesh::GenBuffers();

	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &stiffMapID);
	glBindTexture(GL_TEXTURE_2D, stiffMapID);
	glTextureStorage2D(stiffMapID, 1, GL_RGBA8, stiffnessMap->x, stiffnessMap->y);
	glTextureSubImage2D(stiffMapID, 0, 0, 0, stiffnessMap->x, stiffnessMap->y, GL_RGBA, GL_UNSIGNED_BYTE, stiffnessMap->data);
}

void BushMesh::SetUniforms()
{
	TexturedMesh::SetUniforms();

	GLint stiffTexLoc = glGetUniformLocation(shaderProgram, "stiffness");
	if (stiffTexLoc != -1)
	{
		glUniform1i(stiffTexLoc, 2);
	}

	GLint windLoc = glGetUniformLocation(shaderProgram, "wind");
	if (windLoc != -1)
	{
		glUniform3fv(windLoc, 1, &wind[0]);
	}

	GLint timeLoc = glGetUniformLocation(shaderProgram, "time");
	if (timeLoc != -1)
	{
		glUniform1f(timeLoc, STime::GetCurTime());
	}
}

void BushMesh::SetBufferData()
{
	TexturedMesh::SetBufferData();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, stiffMapID);
}

void BushMesh::SetStiffMap(const char * ifile)
{
	stiffnessMap = new aiu::Texture(ifile);
}
