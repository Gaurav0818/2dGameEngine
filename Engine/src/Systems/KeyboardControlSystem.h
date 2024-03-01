#pragma once

#include <SDL_keycode.h>

#include "../Components/KeyboardControlledComponent.h"
#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventManager/EventManager.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System
{
public:
	KeyboardControlSystem()
	{
		RequireComponent<KeyboardControlledComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManger)
	{
		eventManger->SubscribeToEvent<KeyboardControlSystem, KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		// TODO: change sprite and direction

		for(auto entity: GetSystemEntities())
		{
			const auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

			switch(event.key)
			{
				case SDLK_UP:
					rigidBody.direction = keyboardControl.upVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 0;
					break;
				case SDLK_RIGHT:
					rigidBody.direction = keyboardControl.rightVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 1;
					break;
				case SDLK_DOWN:
					rigidBody.direction = keyboardControl.downVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 2;
					break;
				case SDLK_LEFT:
					rigidBody.direction = keyboardControl.leftVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 3;
					break;

				case SDLK_SPACE:
					
				default:
					break;
			}
			
		}
	}

	void Update()
	{
		
	}
};
