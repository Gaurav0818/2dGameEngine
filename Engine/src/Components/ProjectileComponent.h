#pragma once
#include <SDL_timer.h>

struct ProjectileComponent
{
	bool isFriendly;
	int hitDamage;
	int duration;
	int startTime;

	ProjectileComponent(bool amIPlayer = false, int hitDamage = 0, int duration = 0)
		: isFriendly(amIPlayer), hitDamage(hitDamage), duration(duration), startTime(SDL_GetTicks()) {}
};
