#include <cmath>
#include <iostream>
#include <cstdint>
#include <fmt/core.h>
#include <unordered_set>

#include "sparse_set.hh"
#include "ecs.hh"
#include "interutils.hh"

// HALELELUJA
// int x = 9;
// fmt::print("Apa nr {}", x);

void test_sparse()
{
    auto transform = sparse_set<Vector>(256);

    transform.add(12, {4, 9});
    transform.add(2, {3, -6});

    double dist = vecDist(*transform.get(12), *transform.get(2));

    fmt::print("distance: {}\n", dist);
};

void test_ESC()
{
    ECS* world = new ECS(256);

    uint64_t id = 0;
    std::unordered_set<ComponentType> components;

    components.insert(ComponentType::Player);
    components.insert(ComponentType::Transform);
    components.insert(ComponentType::Physics);

    world->create_entity(id, components);

    auto entity = world->getEntity(0);
    if (entity == nullptr)
    {
        std::cerr << "entity is nullptr" << std::endl;
        return;
    }

    delete world;
};

int main()
{
    test_sparse();
    test_ESC();
};
