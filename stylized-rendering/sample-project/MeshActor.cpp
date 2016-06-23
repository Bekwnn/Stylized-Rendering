#include "MeshActor.h"

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "ShaderLoader.h"
#include "Scene.h"

MeshActor::MeshActor()
{
	scale = glm::vec3(1, 1, 1);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &elementBuffer);
	glGenBuffers(1, &vertexBuffer);
}

void MeshActor::Render()
{
	glUseProgram(shaderProgram);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// bind buffers
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// automatically attempt setting an MVP matrix in shader
	GLuint MVPMatrixID = glGetUniformLocation(shaderProgram, "MVP");
	if (MVPMatrixID != -1)
	{
		glm::mat4 m = GetModelMatrix();
		mvp = scene->camera->GetMVP(m);
		glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &mvp[0][0]);
	}

	// set element buffer with face indices
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		mesh.mFaces.size() * sizeof(glm::ivec3),
		mesh.mFaces.data(),
		GL_STATIC_DRAW
	);

	// set array buffer with vertex positions
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mVertices.size() * sizeof(glm::vec3),
		mesh.mVertices.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	glDrawElements(
		GL_TRIANGLES,
		mesh.mFaces.size() * 3,
		GL_UNSIGNED_INT,
		NULL
	);


	glDisableVertexAttribArray(0);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::string MeshActor::SetMesh(std::string & pFile)
{
	return aiu::Import(mesh, pFile);
}

void MeshActor::SetShader(const char* vertProgram, const char* fragProgram)
{
	shaderProgram = ShaderLoader::CompileVertFrag(vertProgram, fragProgram);
}
