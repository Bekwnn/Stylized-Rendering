#include "Actor.h"

class TestActor : public Actor
{
public:
	TestActor();
	~TestActor();

	virtual void Render() override;

	static const GLfloat g_vertex_buffer_data[];
	static const GLfloat g_color_buffer_data[];

	glm::mat4 mvp;
};