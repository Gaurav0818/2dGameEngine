#include "ECS.h"
#include "../Logger/Logger.h"

void Entity::Kill()
{
    m_registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity)
{
    m_Entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    std::erase_if(m_Entities, [entity](Entity other){return entity == other;});
}

void Registry::Update()
{
    // Add the entities that are waiting to be created to the active system
    for (auto entity : m_entitiesToBeAdded)
        AddEntityToSystems(entity);

    m_entitiesToBeAdded.clear();
    
    // Process the entities that are waiting to be killed from the active Systems.
    for(auto entity: m_entitiesToBeKilled)
    {
        RemoveEntityFromSystems(entity);
        
        // Make the entity id available to be reused
        m_entityComponentSignatures[entity.GetId()].reset();
        m_freeIds.push_back(entity.GetId());
        Logger::Info(" Entity ID:"+std::to_string( entity.GetId())+" is added to freeIdList" );
    }
    
    m_entitiesToBeKilled.clear();
}

Entity Registry::CreateEntity()
{
    int entityId;
    if(m_freeIds.empty())
    {
        entityId = m_numEntities++;
        // Make Sure the entityComponentSignatures vector can accomodate the new entity
        if(entityId >= static_cast<int>(m_entityComponentSignatures.size()))
            m_entityComponentSignatures.resize(entityId + 1);
    }
    else
    {
        // Reuse an id from the list of previously removed entities
        entityId = m_freeIds.front();
        m_freeIds.pop_front();
    }
    
    Entity entity(entityId);
    entity.SetRegistry(this);

    m_entitiesToBeAdded.insert(entity);
    
    Logger::Info("Entity created with ID: " + std::to_string(entityId));
    
    return entity;
}

void Registry::KillEntity(Entity entity)
{
    m_entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity) const
{
    const auto entityId = entity.GetId();

    // m_entityComponentSignatures
    const auto& entityComponentSignature = m_entityComponentSignatures[entityId];
    for (auto& element : m_systems)
    {
        const auto& systemComponentSignature = element.second->GetComponentSignature();
        
        if((entityComponentSignature & systemComponentSignature) == systemComponentSignature)
        {
            // Add the Entity to the System
            element.second->AddEntityToSystem(entity);
        }
    }
}
void Registry::RemoveEntityFromSystems(Entity entity)
{
    for(auto system: m_systems)
        system.second->RemoveEntityFromSystem(entity);
}

