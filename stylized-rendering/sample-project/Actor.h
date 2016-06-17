/// represents an object which exists in space within the scene

#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

class Actor
{
public:
	glm::vec3 position;
	glm::quat rotation;

	virtual void Tick() = 0;
	virtual void Render() {};

	void SetPosition(glm::vec3 val);
	void SetRotation(glm::quat val);

	glm::vec3 GetForwardVec();
	glm::vec3 GetUpVec();
	glm::vec3 GetRightVec();
};