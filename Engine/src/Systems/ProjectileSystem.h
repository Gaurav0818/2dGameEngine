#pragma once

#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"

class ProjectileSystem : public System
{
public:
	ProjectileSystem()
	{
		RequireComponent<ProjectileComponent>();
	}

	void Update()
	{
		for(auto entity: GetSystemEntities())
		{
			auto projectile = entity.GetComponent<ProjectileComponent>();

			// Kill projectile after they reach their duration limit
			if(SDL_GetTicks() - projectile.startTime > projectile.duration)
			{
				entity.Kill();
			}
		}
	}
};
