#pragma once
#include "../ECS/ECS.h"
#include "../Events/KeyPressedEvent.h"
#include "../EventManager/EventManager.h"



class KeyboardInputSystem : public System
{
public:

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyboardInputSystem, KeyPressedEvent>(this, &KeyboardInputSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& e)
	{
		Logger::Info("Key Pressed: [" + std::to_string(e.key) + "] "+ SDL_GetKeyName(e.key));
	}
};
