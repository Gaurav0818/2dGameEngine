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
			
			for (auto it2 = it1 + 1; it2 != entities.end(); ++it2)
			{
				auto& b = *it2;
				auto& bCollider = b.GetComponent<BoxColliderComponent>();
				auto& bTransform = b.GetComponent<TransformComponent>();

				if (CheckAABoxCollision(
					aTransform.position.x + aCollider.offset.x,
					aTransform.position.y + aCollider.offset.y,
					aCollider.width * aTransform.scale.x,
					aCollider.height * aTransform.scale.y,
					bTransform.position.x + bCollider.offset.x,
					bTransform.position.y + bCollider.offset.y,
					bCollider.width * bTransform.scale.x,
					bCollider.height * bTransform.scale.y))
				{
					aCollider.isColliding = true;
					bCollider.isColliding = true;
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
