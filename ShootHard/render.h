#pragma once

#include "types.h"

namespace render
{
    RenderId create_draw_call_id(int _layer, int _depth, int _materialId);
    RenderId create_draw_command_id(int _layer, int _sequence, int _command, void* _data = nullptr);
    bool render_id_less_than(const RenderId* a, const RenderId* b);
}