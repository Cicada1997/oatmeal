#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <unordered_set>

#include "sparse_set.hh"
#include "ecs.hh"

// HALELELUJA
// int x = 9;
// fmt::print("Apa nr {}", x);

struct Vector { double x; double y; };
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

    fmt::print("distance: {}\n", dist);
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
