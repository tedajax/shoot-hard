#include "entity.h"

namespace entity
{
    EntityId currentId = 0;

    EntityId create()
    {
        return currentId++;
    }
}