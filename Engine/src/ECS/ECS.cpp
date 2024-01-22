#include "ECS.h"

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity)
{
    std::erase_if(entities, [entity](Entity other){return entity == other;});
}
