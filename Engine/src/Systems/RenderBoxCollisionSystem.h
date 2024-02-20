#pragma once

#include "../AssetManager/AssetManager.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"


class RenderBoxCollisionSystem : public System
{
public:
	RenderBoxCollisionSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera)//, AssetManager& assetManager)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect colliderRect = {
				static_cast<int>(transform.position.x + collider.offset.x - camera.x),
				static_cast<int>(transform.position.y + collider.offset.y - camera.y),
				static_cast<int>(collider.width * transform.scale.x),
				static_cast<int>(collider.height * transform.scale.y)
			};

			if(collider.isColliding)
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			
			SDL_RenderDrawRect(renderer, &colliderRect);
		}
	}
};
