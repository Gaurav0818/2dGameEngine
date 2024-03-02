#pragma once
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
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

	void OnCollision(CollisionEvent& event)
	{
		auto entityA = event.entityA;
		auto entityB = event.entityB;
		
		Logger::Warning("Collision Detected btw Entity "+std::to_string(entityA.GetId())+" and Entity "+std::to_string(entityB.GetId())+"!");

		if(entityA.BelongsToGroup("projectiles"))
		{
			OnProjectileHitsPlayer(entityA, entityB);
		}
		else if(entityB.BelongsToGroup("projectiles"))
		{
			OnProjectileHitsPlayer(entityB, entityA);
		}
	}

	void OnProjectileHitsPlayer(Entity projectileEntity, Entity receiverEntity)
	{
		auto projectile = projectileEntity.GetComponent<ProjectileComponent>();

		if(!receiverEntity.HasComponent<HealthComponent>())
			return;

		auto& health = receiverEntity.GetComponent<HealthComponent>();
		bool isPLayer = receiverEntity.HasTag("player");
		bool isEnemy = receiverEntity.BelongsToGroup("enemy");
		
		if( isPLayer || isEnemy )
		{
			bool isFriendlyProjectile = projectile.isFriendly;
			
			if(( !isFriendlyProjectile && isPLayer ) || ( isFriendlyProjectile && isEnemy) )
			{
				health.healthPercentage -= projectile.hitDamage;
				projectileEntity.Kill();
			}
			
			if(health.healthPercentage <= 0)
				receiverEntity.Kill();
		}
	}

	void Update()
	{
		
	}
	
};
