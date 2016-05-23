#include "memory_tracker.h"
#include "murmur_hash.h"

namespace core
{
    void MemoryTracker::track_alloc(const foundation::Allocator* _allocator, const void* _p, size_t _bytes, size_t _align)
    {
        if (get_recursion_guard()) {
            return;
        }

        set_recursion_guard(true);

        StackTrace trace;
        get_stack_trace(trace, 3);

        m_mutex.lock();

        AllocationRecord record;
        record.address = _p;
        record.allocator = _allocator;
        record.stackHash = foundation::murmur_hash_64(foundation::array::begin(trace), foundation::array::size(trace), 0);
        record.size = _bytes;
        record.align = _align;

        foundation::hash::set(m_allocationMap, (uint64)_p, record);

        if (!foundation::hash::has(m_stackTraceMap, record.stackHash)) {
            foundation::hash::set(m_stackTraceMap, record.stackHash, trace);
        }

        m_mutex.unlock();
        set_recursion_guard(false);
    }
}