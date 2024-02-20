#pragma once
#include <glm/glm.hpp>

struct KeyboardControlledComponent
{
	glm::vec2 upVelocity;
	glm::vec2 rightVelocity;
	glm::vec2 downVelocity;
	glm::vec2 leftVelocity;
	
	KeyboardControlledComponent(
			glm::vec2 upVec = glm::vec2(0,-1) ,
			glm::vec2 rightVec = glm::vec2(1,0),
			glm::vec2 downVec = glm::vec2(0,1),
			glm::vec2 leftVec = glm::vec2(-1,0) )
		: upVelocity(upVec), rightVelocity(rightVec), downVelocity(downVec), leftVelocity(leftVec) {}
};
