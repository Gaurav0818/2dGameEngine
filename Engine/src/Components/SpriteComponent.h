#pragma once

struct SpriteComponent
{
	std::string id;
	int width;
	int height;
	SDL_Rect srcRect;

	SpriteComponent(std::string textureId = "", int width = 0, int height = 0, int srcRectX = 0, int srcRectY = 0)
		: id(textureId), width(width), height(height), srcRect({srcRectX, srcRectY, width, height}) {}
};
