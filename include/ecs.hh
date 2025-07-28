#ifndef ESC_H
#define ESC_H
 
#include <cstdint>
#include <fmt/core.h>
#include <unordered_set>
#include "sparse_set.hh"

enum class ComponentType {
    Transform,
    Physics,
    Render,
    Player,
};

typedef struct {
    uint64_t id;
    std::unordered_set<ComponentType> components;
} Entity;

class ECS
{
    sparse_set<std::unordered_set<ComponentType>> World;

    public:
    ECS(uint64_t MAX_ENTITIES)
        : World(MAX_ENTITIES)
    {
        fmt::print("ECS initialized. Max entities: {}\n", MAX_ENTITIES);
    };

    void create_entity(uint64_t entityID, std::unordered_set<ComponentType> components);
    std::unordered_set<ComponentType> getEntity(uint64_t entityID);

    // @brief Gets the entity from the world, then appends the
    // @brief enum class representing the component to the world.
    void AddComponent(uint64_t entityID, ComponentType component);

    // @brief Gets the entity from the world, then deletes the
    // @brief enum class representing the component from the world.
    void withdrawComponent(uint64_t entityID, ComponentType component);

    void removeEntity(uint64_t entityID);
};

#endif
