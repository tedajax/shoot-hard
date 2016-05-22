#include "transform.h"

namespace transform
{
    Transform identity()
    {
        Transform result;
        result.position = glm::vec2(0.f, 0.f);
        result.scale = glm::vec2(1.f, 1.f);
        result.rotation = 0.f;
        return result;
    }
}