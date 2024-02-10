#pragma once

#include<SDL.h>

struct AnimationComponent
{
	int numFrames;
	int currentFrame;
	int frameSpeedRate;
	bool isLooping;
	int startTime;

	AnimationComponent(int numFrames = 0, int frameSpeedRate = 0, bool isLooping = false)
		: numFrames(numFrames), currentFrame(1), frameSpeedRate(frameSpeedRate), isLooping(isLooping), startTime(SDL_GetTicks()) {}
};
