#pragma once
#include "../EventManager/Event.h"

class KeyPressedEvent: public Event
{
public:
	SDL_Keycode key;
	KeyPressedEvent(SDL_Keycode key): key(key) {}
};
