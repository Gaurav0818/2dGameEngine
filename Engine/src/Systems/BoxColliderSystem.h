#pragma once

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class BoxColliderSystem: public System
{
public:
	BoxColliderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update()
	{
		// TODO:
		// Check all entity that has box collider
		// to see if they have box collider
		for (auto entity1: GetSystemEntities())
		{
			for (auto entiry2: GetSystemEntities())
			{
				if (entity1 == entiry2)
					continue;
				
				
				auto& collider1 = entity1.GetComponent<BoxColliderComponent>();
				auto& transform1 = entity1.GetComponent<TransformComponent>();
				
				auto& collider2 = entiry2.GetComponent<BoxColliderComponent>();
				auto& transform2 = entiry2.GetComponent<TransformComponent>();
				
				if (transform1.position.x < transform2.position.x + collider2.width * transform2.scale.x &&
					transform1.position.x + transform1.scale.x * collider1.width > transform2.position.x  &&
					transform1.position.y < transform2.position.y + collider2.height * transform2.scale.y &&
					transform1.position.y + transform1.scale.y * collider1.height > transform2.position.y )
				{
					Logger::Info("Collision detected");
				}
			}
		}
	}
};
