#pragma once

struct SpriteComponent
{
	std::string id;
	SDL_Rect srcRect;
	unsigned int zIndex;

	SpriteComponent(std::string textureId = "", int width = 0, int height = 0, unsigned int zIndex = 0, int srcRectX = 0, int srcRectY = 0)
		: id(textureId), srcRect({srcRectX, srcRectY, width, height}), zIndex(zIndex) {}
};
