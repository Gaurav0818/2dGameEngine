#pragma once
#include "../Components/BoxColliderComponent.h"
#include "../ECS/ECS.h"

#include "../Events/CollisionEvent.h"
#include "../EventManager/EventManager.h"

class DamageSystem: public System
{
public:
	DamageSystem()
	{
		RequireComponent<BoxColliderComponent>();
	}

	// Subscribe to CollisionEvent
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<DamageSystem, CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& e)
	{
		auto entityA = e.entityA;
		auto entityB = e.entityB;
		
		auto& aCollider = entityA.GetComponent<BoxColliderComponent>();
		auto& bCollider = entityB.GetComponent<BoxColliderComponent>();
		
		if(aCollider.isColliding && bCollider.isColliding)
		{
			Logger::Warning("Collision Detected btw Entity "+std::to_string(entityA.GetId())+" and Entity "+std::to_string(entityB.GetId())+"!");
			//entityA.Kill();
			//entityB.Kill();
		}
	}

	void Update()
	{
		
	}
	
};
