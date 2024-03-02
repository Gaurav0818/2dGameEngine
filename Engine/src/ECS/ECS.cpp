#include "ECS.h"
#include "../Logger/Logger.h"

void Entity::Kill()
{
    m_registry->KillEntity(*this);
}
void Entity::Tag(const std::string& tag)
{
    m_registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag)
{
    return m_registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& group)
{
    m_registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group)
{
    return m_registry->EntityBelongsToGroup(*this, group);
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

        // Remove any traces of the entity from the tag/group maps
        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
        
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

void Registry::TagEntity(Entity entity, const std::string& tagToAssign)
{
    m_entityPerTag.emplace(tagToAssign, entity);
    m_tagPerEntity.emplace(entity.GetId(), tagToAssign);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tagToCheck)
{
    if(m_tagPerEntity.find(entity.GetId()) == m_tagPerEntity.end())
        return false;
    
    return m_entityPerTag.find(tagToCheck)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string& tag) const
{
    return m_entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity)
{
    auto taggedEntity = m_tagPerEntity.find(entity.GetId());
    if( taggedEntity != m_tagPerEntity.end())
    {
        auto tag = taggedEntity->second;
        m_entityPerTag.erase(tag);
        m_tagPerEntity.erase(taggedEntity);
    }
}

void Registry::GroupEntity(Entity entity, const std::string& groupToAssign)
{
    m_entitiesPerGroup.emplace(groupToAssign, std::set<Entity>());
    m_entitiesPerGroup[groupToAssign].emplace(entity);
    m_groupPerEntity.emplace(entity.GetId(), groupToAssign);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& groupToCheck) const 
{
    if(m_entitiesPerGroup.find(groupToCheck) == m_entitiesPerGroup.end())
    {
        Logger::Error("Group type does not Exist : "+ groupToCheck);
        return false;
    }
    
    auto groupEntities = m_entitiesPerGroup.at(groupToCheck);
    return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntityByGroup(const std::string& group) const
{
    auto& setOfEntities = m_entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity)
{
    auto groupedEntity = m_groupPerEntity.find(entity.GetId());
    if(groupedEntity != m_groupPerEntity.end())
    {
        auto group = m_entitiesPerGroup.find(groupedEntity->second);
        if(group != m_entitiesPerGroup.end())
        {
            auto entityInGroup = group->second.find(entity);
            if(entityInGroup != group->second.end())
                group->second.erase(entityInGroup);
        }
        m_groupPerEntity.erase(groupedEntity);
    }
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

