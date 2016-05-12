#pragma once

#include "types.h"

namespace render
{
    class RenderQueue
    {
        RenderQueue(foundation::Allocator& _allocator);
        ~RenderQueue();

        foundation::Array<RenderId> renderCallKeys;
        foundation::Array<RenderCommandData> renderCallValues;
    };
}