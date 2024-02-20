#pragma once

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/CollisionEvent.h"

class BoxColliderSystem: public System
{
public:
	BoxColliderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update(std::unique_ptr<EventManager>& eventManager, SDL_Rect& camera)
	{
		auto entities = GetSystemEntities();

		for(auto entity: entities)
		{
			auto& aCollider = entity.GetComponent<BoxColliderComponent>();
			aCollider.isColliding = false;
		}
		
		for (auto it1 = entities.begin(); it1 != entities.end(); ++it1)
		{
			auto& a = *it1;
			auto& aCollider = a.GetComponent<BoxColliderComponent>();
			auto& aTransform = a.GetComponent<TransformComponent>();

			glm::vec2 aPos = glm::vec2(
				aTransform.position.x - (aTransform.isFixed ? 0 : camera.x),
				aTransform.position.y - (aTransform.isFixed ? 0 : camera.y));
			
			for (auto it2 = it1 + 1; it2 != entities.end(); ++it2)
			{
				auto& b = *it2;
				auto& bCollider = b.GetComponent<BoxColliderComponent>();
				auto& bTransform = b.GetComponent<TransformComponent>();
				
				glm::vec2 bPos = glm::vec2(
					bTransform.position.x - (bTransform.isFixed ? 0 : camera.x),
					bTransform.position.y - (bTransform.isFixed ? 0 : camera.y));
				
				if (CheckAABoxCollision(
					aPos.x + aCollider.offset.x ,
					aPos.y + aCollider.offset.y,
					aCollider.width * aTransform.scale.x,
					aCollider.height * aTransform.scale.y,
					bPos.x + bCollider.offset.x,
					bPos.y + bCollider.offset.y,
					bCollider.width * bTransform.scale.x,
					bCollider.height * bTransform.scale.y))
				{
					aCollider.isColliding = true;
					bCollider.isColliding = true;

					// Create Collision Event
					eventManager->EmitEvent<CollisionEvent>(a , b);
				}
			}
		}
	}

	bool CheckAABoxCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
	{
		return{	aX < bX + bW &&
				aX + aW > bX &&
				aY < bY + bH &&
				aY + aH > bY };
	}

};
