#include "render.h"

namespace render
{
    void internal_create_id_header(uint64 _layer, uint64 _cmd, RenderId& _out)
    {
        _out.id = 0;
        _out.id = ((_layer & 0x7F) << 57) + 
            ((_cmd & 0x1) << 56);
    }

    RenderId create_draw_call_id(int _layer, int _depth, int _materialId)
    {
        RenderId result;

        internal_create_id_header(_layer, 0, result);

        result.id += (((uint64)_depth & 0xFFFFFF) << 32) +
            (_materialId & 0xFFFFFFFF);
        result.data = nullptr;

        return result;
    }

    RenderId create_draw_command_id(int _layer, int _sequence, int _command, void* _data /* = nullptr */)
    {
        RenderId result;

        internal_create_id_header(_layer, 1, result);

        result.id = (((uint64)_sequence & 0xFFFFFF) << 32) +
            (_command & 0xFFFFFFFF);
        result.data = _data;

        return result;
    }

    bool render_id_less_than(const RenderId* a, const RenderId* b)
    {
        return (a->id < b->id) ||
            ((a->id == b->id) && (*(uint64*)a->data < *(uint64*)b->data));
    }
}