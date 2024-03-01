#pragma once

#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class ProjectileEmitSystem : public System
{
public:
	ProjectileEmitSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileEmitterComponent>();
	}

	void Update(std::unique_ptr<Registry> &registry)
	{
		for (auto entity: GetSystemEntities())
		{
			auto& projectile = entity.GetComponent<ProjectileEmitterComponent>();
			const auto transform = entity.GetComponent<TransformComponent>();
			
			// Check if it's time to emit a projectile
			if(SDL_GetTicks() - projectile.lastEmissionTime > projectile.repeatFrequency)
			{
				glm::vec2 projectilePosition = transform.position;
				if(entity.HasComponent<SpriteComponent>())
				{
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += transform.scale.x * sprite.srcRect.w / 2;
					projectilePosition.y += transform.scale.y * sprite.srcRect.h / 2;
				}
				// Emit a projectile
				// Add a new projectile entity to the registry
				Entity projectileEnitiy = registry->CreateEntity();
				projectileEnitiy.AddComponent<TransformComponent>(projectilePosition, transform.scale, 0, 1);
				projectileEnitiy.AddComponent<RigidBodyComponent>(projectile.direction, projectile.speed);
				projectileEnitiy.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectileEnitiy.AddComponent<BoxColliderComponent>(4, 4 , glm::vec2(0,0));

				projectile.lastEmissionTime = SDL_GetTicks();
			}
		}
	}
};
