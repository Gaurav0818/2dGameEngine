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
		// TODO: change sprite and velocity

		for(auto entity: GetSystemEntities())
		{
			const auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

			switch(event.key)
			{
				case SDLK_UP:
					rigidBody.velocity = keyboardControl.upVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 0;
					break;
				case SDLK_RIGHT:
					rigidBody.velocity = keyboardControl.rightVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 1;
					break;
				case SDLK_DOWN:
					rigidBody.velocity = keyboardControl.downVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 2;
					break;
				case SDLK_LEFT:
					rigidBody.velocity = keyboardControl.leftVelocity;
					sprite.srcRect.y = sprite.srcRect.h * 3;
					break;

				default:
					break;
			}
			
		}
	}

	void Update()
	{
		
	}
};
