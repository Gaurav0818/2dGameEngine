#pragma once

#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
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

	bool forceEmmit = false;

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManger)
	{
		eventManger->SubscribeToEvent<ProjectileEmitSystem, KeyPressedEvent>(this, &ProjectileEmitSystem::OnSpacePressed);
	}

	void OnSpacePressed(KeyPressedEvent& event)
	{
		if(event.key == SDLK_SPACE)
			forceEmmit = true;
	}


	void Update(SDL_Rect& camera)
	{
		EmitProjectile(camera);
	}

	void EmitProjectile(SDL_Rect& camera)
	{
		for (auto entity: GetSystemEntities())
		{
			auto& projectileEmitterComponent = entity.GetComponent<ProjectileEmitterComponent>();
			const auto transform = entity.GetComponent<TransformComponent>();

			if(projectileEmitterComponent.repeatFrequency == 0 && !forceEmmit)
				continue;
			
			// Check if it's time to emit a projectile
			if(SDL_GetTicks() - projectileEmitterComponent.lastEmissionTime > projectileEmitterComponent.repeatFrequency)
			{
				glm::vec2 projectilePosition = glm::vec2(
					transform.position.x - (transform.isFixed ? 0 : camera.x),
					transform.position.y - (transform.isFixed ? 0 : camera.y));
				if(entity.HasComponent<SpriteComponent>())
				{
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += transform.scale.x * sprite.srcRect.w / 2;
					projectilePosition.y += transform.scale.y * sprite.srcRect.h / 2;
				}

				if(entity.HasComponent<RigidBodyComponent>())
				{
					const auto rb = entity.GetComponent<RigidBodyComponent>();
					if(rb.direction == glm::vec2(0))
						projectileEmitterComponent.direction = glm::vec2(0,-1);
					else
						projectileEmitterComponent.direction = rb.direction;
				}
				// Emit a projectile
				// Add a new projectile entity to the registry
				Entity projectileEnitiy = entity.GetRegistry()->CreateEntity();
				projectileEnitiy.AddComponent<TransformComponent>(projectilePosition, transform.scale, 0, 1);
				projectileEnitiy.AddComponent<RigidBodyComponent>(projectileEmitterComponent.direction,
					projectileEmitterComponent.speed);
				projectileEnitiy.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectileEnitiy.AddComponent<BoxColliderComponent>(4, 4 , glm::vec2(0,0));
				projectileEnitiy.AddComponent<ProjectileComponent>(projectileEmitterComponent.isFriendly,
					projectileEmitterComponent.Damage,projectileEmitterComponent.projectileDuration);				
				projectileEmitterComponent.lastEmissionTime = SDL_GetTicks();
			}
		}
		forceEmmit = false;
	}
};
