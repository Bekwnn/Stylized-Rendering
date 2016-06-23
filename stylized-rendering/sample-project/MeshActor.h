#include "Actor.h"

#include <assimp\scene.h>
#include "AssimpUtil.h"

class MeshActor : public Actor
{
public:
	MeshActor();
	void Render() override;
	std::string SetMesh(std::string& pFile);
	void SetShader(const char* vertProgram, const char* fragProgram);

	aiu::Mesh mesh;
	glm::mat4 mvp;

	GLuint vao;
	GLuint shaderProgram;
	GLuint elementBuffer;
	GLuint vertexBuffer;
};