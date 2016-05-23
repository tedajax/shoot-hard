#pragma once

#include "platform.h"
#include "debug.h"
#include "array.h"
#include "hash.h"
#include "string_stream.h"
#include <mutex>

namespace core
{
    class MemoryTracker
    {
    public:
        MemoryTracker(foundation::Allocator& _allocator)
            : m_allocationMap(_allocator),
            m_stackTraceMap(_allocator)
        {
            foundation::hash::reserve(m_allocationMap, 10000);
            foundation::hash::reserve(m_stackTraceMap, 10000);
        }

        ~MemoryTracker()
        { }

        struct AllocationRecord
        {
            const void* address;
            const foundation::Allocator* allocator;
            size_t size;
            size_t align;
            uint64 stackHash;
        };

        using AllocationMapType = foundation::Hash<AllocationRecord>;
        using StackTraceMapType = foundation::Hash<StackTrace>;

        class Snapshot
        {
        public:
            Snapshot(foundation::Allocator& _allocator)
                : m_allocationMap(_allocator)
            {
                foundation::hash::reserve(m_allocationMap, 10000);
            }

            inline ~Snapshot();

            void set_diff(const Snapshot& _first, const Snapshot& _second);
            void print_leaves(foundation::string_stream::Buffer& _outBuffer) const;
            uint32 get_count() const { return foundation::hash::size(m_allocationMap); }

        private:
            friend MemoryTracker;

            inline void set_data(const AllocationMapType& _allocations, const StackTraceMapType* _stackTraceMap);
            void append_allocation(const AllocationRecord* allocations, foundation::string_stream::Buffer& _outBuffer) const;

            AllocationMapType m_allocationMap;
            const StackTraceMapType* m_stackTraceMap;
        };

        inline void take_snapshot(Snapshot* _snapshot);

        const AllocationRecord* get_allocation_record(const foundation::Allocator* _allocator, const void* _p) const;
        const StackTrace* getStackTraceForAllocation(const foundation::Allocator* _allocator, const void* _p) const;

        void track_alloc(const foundation::Allocator* _allocator, const void* _p, size_t _bytes, size_t _align);
        void track_free(const foundation::Allocator* _allocator, const void* _p, size_t _bytes, size_t _align);
        void track_realloc(const foundation::Allocator* _allocator, const void* _p, size_t _oldSize, size_t _newSize);

        CORE_FORCE_INLINE void set_recursion_guard(bool enable);

    private:
        CORE_FORCE_INLINE bool get_recursion_guard();

        std::mutex m_mutex;

        static thread_local bool s_recursionGuard;

        AllocationMapType m_allocationMap;
        StackTraceMapType m_stackTraceMap;
    };

    inline void MemoryTracker::Snapshot::set_data(const AllocationMapType& _allocations, const StackTraceMapType* _stackTraceMap)
    {
        foundation::hash::clear(m_allocationMap);
        const auto* allocation = foundation::hash::begin(_allocations);
        while (allocation != nullptr) {
            foundation::hash::set(m_allocationMap, (uint64)allocation->value.address, allocation->value);
            allocation = foundation::hash::walk_next(_allocations, allocation);
        }
        m_stackTraceMap = _stackTraceMap;
    }

    inline MemoryTracker::Snapshot::~Snapshot()
    {
        MemoryTracker* tracker = foundation::memory::get_memory_tracker();
        if (tracker) {
            tracker->set_recursion_guard(true);
            foundation::hash::clear(m_allocationMap);
            tracker->set_recursion_guard(false);
        }
    }

    inline void MemoryTracker::take_snapshot(Snapshot* _snapshot)
    {
        MemoryTracker* tracker = foundation::memory::get_memory_tracker();
        ASSERT(tracker != nullptr, "No memory tracker available.");
        std::lock_guard<std::mutex> lock(tracker->m_mutex);
        tracker->set_recursion_guard(true);
        _snapshot->set_data(tracker->m_allocationMap, &tracker->m_stackTraceMap);
        tracker->set_recursion_guard(false);
    }

    CORE_FORCE_INLINE void MemoryTracker::set_recursion_guard(bool enable)
    {
        s_recursionGuard = enable;
    }

    CORE_FORCE_INLINE bool MemoryTracker::get_recursion_guard()
    {
        return s_recursionGuard;
    }
}