#pragma once
#include "../Game/Game.h"
#include "../ECS/ECS.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class CameraMovementSystem : public System
{
public:
	CameraMovementSystem()
	{
		RequireComponent<CameraFollowComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(SDL_Rect& camera)
	{
		for (auto entity: GetSystemEntities())
		{
			auto& transform = entity.GetComponent<TransformComponent>();

			// Center the camera over the player
			if(transform.position.x + (camera.w/2) < Game::mapWidth)
				camera.x = static_cast<int>(transform.position.x - (Game::winWidth/2));
			
			if(transform.position.y + (camera.h/2)< Game::mapHeight)
				camera.y = static_cast<int>(transform.position.y - (Game::winHeight/2));

			// Clamp the camera to the map
			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;
			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = camera.y > camera.h ? camera.h : camera.y;
		}
	}
};