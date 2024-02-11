#pragma once
#include <glm/vec2.hpp>

struct BoxColliderComponent
{
	float width;
	float height;
	glm::vec2 offset;

	bool isColliding = false;

	BoxColliderComponent(float width = 0, float height =0, glm::vec2 offset = glm::vec2(0))
		: width(width), height(height), offset(offset) {}
};
