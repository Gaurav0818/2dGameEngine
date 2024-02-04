#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec2 position;
	double rotation;
	glm::vec2 scale;

	TransformComponent(glm::vec2 position = glm::vec2(0,0),
					glm::vec2 scale = glm::vec2(0,0),
					double rotation = 0.0)
					: position(position), rotation(rotation), scale(scale) {}
};
