#include <bitset>
#include <vector>

typedef unsigned long long EntityID;
const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

namespace kecs
{
    /*Entity infomation*/
    struct EntityDecs
    {
        EntityID id;
        ComponentMask mask;
    };
    std::vector<EntityDecs> entities;

    EntityID NewEntity()
    {
        entities.push_back({entities.size(), ComponentMask()});
        return entities.back().id;
    }

}