#pragma once

struct SpriteComponent
{
	std::string textureId;
	int width;
	int height;

	SpriteComponent(std::string textureId = " ", int width = 0, int height = 0)
		: textureId(std::move(textureId)), width(width), height(height) {}
};
