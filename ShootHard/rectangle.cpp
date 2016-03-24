#include "rectangle.h"

namespace rectangle
{
    Rectangle default_uvs()
    {
        Rectangle r;
        r.position.x = 0.f;
        r.position.y = 0.f;
        r.width = 1.f;
        r.height = 1.f;
        return r;
    }
}