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
    // TODO: Add Logic to Add and Remove Entities
}

Entity Registry::CreateEntity()
{
    int entityID = m_numEntities++;
    
    Entity entity(entityID);
    m_entitiesToBeAdded.insert(entity);

    Logger::Info("Entity created with ID: " + std::to_string(entityID));
    
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

