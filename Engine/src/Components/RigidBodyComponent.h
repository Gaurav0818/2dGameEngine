#pragma once

#include <glm/glm.hpp>

struct RigidBodyComponent
{
	float speed;
	glm::vec2 direction;

	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0,0), float speed = 0)
		: speed(speed), direction(velocity) {}
};
