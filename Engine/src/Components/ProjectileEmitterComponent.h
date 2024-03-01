#pragma once 

#include <glm/glm.hpp>
#include <SDL.h>

struct ProjectileEmitterComponent
{
    glm::vec2 direction;
    int speed;
    int repeatFrequency;
    int projectileDuration;
    int Damage;
	bool isFriendly;
    int lastEmissionTime;
	
	ProjectileEmitterComponent( glm::vec2 dir = glm::vec2(0), int speed = 1,
								int repeatFreq = 0, int duration = 1000, int dmg = 0, bool isFriendly = false)
			: direction(dir), speed(speed), repeatFrequency(repeatFreq), projectileDuration(duration),
								Damage(dmg), isFriendly(isFriendly), lastEmissionTime(SDL_GetTicks()) {}

	
	
};