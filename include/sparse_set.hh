#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <limits>
#include <utility>
#include <vector>

template <class T>
class sparse_set
{
    static constexpr uint64_t npos = std::numeric_limits<uint64_t>::max();

    protected:
    std::vector<uint64_t> sparse;    // @brief stores dense indexes
                                     // @index entityID
                                     // @return uint64_t denseIndex
    std::vector<uint64_t> reversed;
    std::vector<T> dense;            // @brief Stores the
                                     // unordered_set of components
                                     // for the id
                                     // @index uint64_t
                                     // @return unordered_set<ComponentType>

    public:
    sparse_set(uint64_t MAX_ENTITIES) 
        : sparse(MAX_ENTITIES, npos)
          , dense()
          , reversed()
    {

    };

    void add(uint64_t entityID, T component)
    {
        sparse[entityID] = dense.size();
        reversed.push_back(entityID);
        dense.push_back(std::move(component));
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
        uint64_t index        = sparse[entityID];

        uint64_t lastIndex    = dense.size() -1;
        uint64_t lastEntityID = reversed[lastIndex];

        if (index == npos) return;

        std::swap(dense[index],         dense[lastIndex]);
        std::swap(reversed[index],      reversed[lastIndex]);

        sparse[lastEntityID] = index;

        dense.pop_back();
        reversed.pop_back();

        sparse[entityID] = npos;
    };
};

#endif
