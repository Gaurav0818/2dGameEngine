#pragma once

#include <algorithm>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class RenderSystem: public System
{
public:
	RenderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		// Get all system entities
		auto entities = GetSystemEntities();
		
		// Sort entities by z-index
		std::sort(entities.begin(), entities.end(), [](const Entity& a, const Entity& b)
		{
			const auto spriteA = a.GetComponent<SpriteComponent>();
			const auto spriteB = b.GetComponent<SpriteComponent>();
			return spriteA.zIndex < spriteB.zIndex;
		});

		for(auto entity : entities)
		{
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			SDL_Rect srcRect = sprite.srcRect;
			
			SDL_Rect destRect =
				{
					static_cast<int>(transform.position.x) - (transform.isFixed ? 0 : camera.x ),
					static_cast<int>(transform.position.y) - (transform.isFixed ? 0 : camera.y ),
					static_cast<int>(transform.scale.x * static_cast<float>(sprite.srcRect.w)),
					static_cast<int>(transform.scale.y * static_cast<float>(sprite.srcRect.h))
				};
			

			SDL_RenderCopyEx(
				renderer,
				assetManager->GetTexture(sprite.id),
				&srcRect, &destRect,
				transform.rotation,
				nullptr,
				SDL_FLIP_NONE
				);
		}
	}
};
