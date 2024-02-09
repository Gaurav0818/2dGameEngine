#include "ECS.h"
#include "../Logger.h"

void System::AddEntityToSystem(Entity entity)
{
    m_Entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity)
{
    std::erase_if(m_Entities, [entity](Entity other){return entity == other;});
}

void Registry::Update()
{
    // Add the entities that are waiting to be created to the active system
    for (auto entity : m_entitiesToBeAdded)
        AddEntityToSystems(entity);

    m_entitiesToBeAdded.clear();

    // TODO: Add Logic to Remove Entities
}

Entity Registry::CreateEntity()
{
    int entityId = m_numEntities++;
    
    Entity entity(entityId);
    entity.SetRegistry(this);

    m_entitiesToBeAdded.insert(entity);

    // Make Sure the entityComponentSignatures vector can accomodate the new entity
    if(entityId >= static_cast<int>(m_entityComponentSignatures.size()))
        m_entityComponentSignatures.resize(entityId + 1);
    
    Logger::Info("Entity created with ID: " + std::to_string(entityId));
    
    return entity;
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

