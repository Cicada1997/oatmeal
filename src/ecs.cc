#include "ecs.hh"

void ECS::create_entity(uint64_t entityID, std::unordered_set<ComponentType> components) 
{
    World.add(entityID, components);
};

std::unordered_set<ComponentType>* ECS::getEntity(uint64_t entityID) 
{
    auto entity = World.get(entityID);
    return entity;
};

// @brief Gets the entity from the world, then appends the
// @brief enum class representing the component to the world.
void ECS::AddComponent(uint64_t entityID, ComponentType component)
{
    auto entityComponents = *World.get(entityID);
    entityComponents.insert(component);
};

// @brief Gets the entity from the world, then deletes the
// @brief enum class representing the component from the world.
void ECS::withdrawComponent(uint64_t entityID, ComponentType component)
{
    auto entityComponents = *World.get(entityID);
    entityComponents.erase(component);
};

void ECS::removeEntity(uint64_t entityID)
{
    World.remove(entityID);
}
