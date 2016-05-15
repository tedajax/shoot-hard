#pragma once

#include "debug.h"

#define CORE_NEW_PROHIBITED() \
    static CORE_STATIC_ANALYSIS_NON_NULL void* operator new(size_t size) = delete; \
    static CORE_STATIC_ANALYSIS_NON_NULL void* operator new[](size_t size) = delete; \
    static CORE_STATIC_ANALYSIS_NON_NULL void* operator new(size_t size, void* p) = delete; \
    static void* operator new[](size_t size, void* p) = delete; \
    static void operator delete(void* p, size_t size) CORE_NOEXCEPT { ASSERT(false, "Delete called on class for which new is prohibited."); } \
    static void operator delete[](void* p, size_t size) CORE_NOEXCEPT { ASSERT(false, "Delete called on class for which new is prohibited."); } 