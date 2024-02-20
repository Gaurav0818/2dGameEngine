#pragma once

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"

class MovementSystem: public System
{
public:
	MovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update(double deltaTime)
	{
		// Loop All Entities that the system is interested in
		for (auto entity:GetSystemEntities())
		{
			// For each entity, get the components and update them
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
			transform.position.x += rigidBody.velocity.x * rigidBody.speed * deltaTime;
			transform.position.y += rigidBody.velocity.y * rigidBody.speed * deltaTime;
		}
	}
};
