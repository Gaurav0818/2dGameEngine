#include <utility>

#pragma once

struct SpriteComponent
{
	std::string id;
	SDL_Rect srcRect;
	bool isFixed;
	unsigned int zIndex;

	SpriteComponent(std::string textureId = "", int width = 0, int height = 0, unsigned int zIndex = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0)
		: id(std::move(textureId)), srcRect({srcRectX, srcRectY, width, height}), isFixed(isFixed), zIndex(zIndex) {}
};
