#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <unordered_set>

#include "sparse_set.hh"

// HALELELUJA
// int x = 9;
// fmt::print("Apa nr {}", x);

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
        fmt::print("ECS initialized");
    };

    void create_entity(uint64_t entityID, std::unordered_set<ComponentType> components) 
    {
        World.add(entityID, components);
    };

    std::unordered_set<ComponentType> getEntity(uint64_t entityID) 
    {
        return *World.get(entityID);
    };

    // @brief Gets the entity from the world, then appends the
    // @brief enum class representing the component to the world.
    void AddComponent(uint64_t entityID, ComponentType component)
    {
        auto entityComponents = *World.get(entityID);
        entityComponents.insert(component);
    };

    // @brief Gets the entity from the world, then deletes the
    // @brief enum class representing the component from the world.
    void withdrawComponent(uint64_t entityID, ComponentType component)
    {
        auto entityComponents = *World.get(entityID);
        entityComponents.erase(component);
    };

    void removeEntity(uint64_t entityID)
    {
        World.remove(entityID);
    }
};

struct Vector
{
    double x;
    double y;
};

double dabs(double x) { return x >= 0 ? x : x*-1; }; // @brief calculates absolute value for doubles.

double vecDist(Vector pos1, Vector pos2)
{
    double dx2 = dabs(pos1.x - pos2.x) * dabs(pos1.x - pos2.x);
    double dy2 = dabs(pos1.y - pos2.y) * dabs(pos1.y - pos2.y);
    return sqrt(dx2 + dy2);
};

void test_sparse()
{
    auto transform = sparse_set<Vector>(256);

    transform.add(12, {4, 9});
    transform.add(2, {3, -6});

    double dist = vecDist(*transform.get(12), *transform.get(2));

    fmt::print("distance: {}", dist);
};

void test_ESC()
{
    ECS* world = new ECS(256);

    Entity e;

    e.id = 0;
    e.components.insert(ComponentType::Player);
    e.components.insert(ComponentType::Transform);
    e.components.insert(ComponentType::Physics);

    world->create_entity(e.id, e.components);

    delete world;
};

int main()
{
    test_sparse();
    test_ESC();
};
