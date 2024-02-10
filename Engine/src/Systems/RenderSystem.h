#pragma once

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

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager)
	{
		for(auto entity : GetSystemEntities())
		{
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			SDL_Rect srcRect = sprite.srcRect;
			
			SDL_Rect destRect =
				{
					static_cast<int>(transform.position.x),
					static_cast<int>(transform.position.y),
					static_cast<int>(sprite.width * transform.scale.x),
					static_cast<int>(sprite.height * transform.scale.y)
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
