#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <limits>
#include <unordered_set>
#include <vector>

// HALELELUJA
// int x = 9;
// fmt::print("Apa nr {}", x);

using namespace std;

template <class T>
class SparseSet
{
    static constexpr uint64_t npos = numeric_limits<uint64_t>::max();

    protected:
        vector<uint64_t> sparse; // @brief stores dense indexes
                                 // @index entityID
                                 // @return uint64_t denseIndex
        vector<uint64_t> reversed;
        vector<T> dense;          // @brief Stores the
                                  // unordered_set of components
                                  // for the id
                                  // @index uint64_t
                                  // @return unordered_set<ComponentType>
    
    public:
        void add(uint64_t entityID, T component)
        {
            sparse[entityID] = dense.size();
            reversed[dense.size()] = entityID;
            dense.push_back(component);
        };

        T* get(uint64_t entityID)
        {
            uint64_t index = sparse[entityID];

            if (index == npos)
                return nullptr;

            return &dense[index];
        };

        void remove(uint64_t entityID)
        {
            uint64_t index = sparse[entityID];
            if (index == npos)
                return;

            swap(dense.back(), dense[index]);
            swap(reversed.back(), reversed[index]);

            sparse[reversed[index]] = index;

            dense.pop_back();
            reversed.pop_back();

            sparse[entityID] = npos;
        };
};


enum class ComponentType {
    Transform,
    Physics,
    Render,
    Player,
};

typedef struct {
    uint64_t id;
    unordered_set<ComponentType> components;
} Entity;

class ECS
{
    uint64_t m_UserID;
    SparseSet<unordered_set<ComponentType>> World;

    public:
        void create_entity(uint64_t entityID, unordered_set<ComponentType> components) 
        {
            World.add(entityID, components);
        };

        unordered_set<ComponentType> getEntity(uint64_t entityID) 
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

int main()
{
    ECS world;

    Entity e;
    e.id = 0;
    e.components.insert(ComponentType::Player);

    world.create_entity(e.id, e.components);
}
