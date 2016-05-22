#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
#   define COMPILER_MSVC_COMPAT
#elif defined(__GNUC__)
#   define COMPILER_GCC_COMPAT
#endif

#if defined(COMPILER_MSVC_COMPAT)
#pragma warning(disable: 4100)
#pragma warning(disable: 4521)
#pragma warning(disable: 4351)

#define CORE_POINTER_SIZE sizeof(void*)

#if defined(_M_X64) || defined(__amd64__)
#define CORE_IS_64_BIT
#else
#define CORE_IS_32_BIT
#endif

#define CORE_ALIGNOF(t) __alignof(t)
#define CORE_ALIGN(v) __declspec(align(v))
#define CORE_ALIGN_4 __declspec(align(4))
#define CORE_ALIGN_8 __declspec(align(8))
#define CORE_ALIGN_16 __declspec(align(16))

#define CORE_FORCE_INLINE __forceinline

#define CORE_NO_WARNING(b) core::internal::no_warning(b)

namespace core { namespace internal { template <typename T> CORE_FORCE_INLINE T no_warning(T x) { return x; } } }

#define CORE_BREAKPOINT() __debugbreak()
#define CORE_STATIC_ASSERT_TEMPLATE(exp, msg, param) static_assert(exp, msg)
#define CORE_STATIC_ANALYSIS_ASSUME(exp) __analysis_assume(exp)
#define CORE_STATIC_ANALYSIS_NON_NULL _Ret_notnull_
#define CORE_DECLARE_UNREACHABLE() __assume(0)
#define CORE_NOEXCEPT noexcept
#define CORE_NO_INLINE __declspec(noinline)
#define CORE_FUNC_NAME __FUNCSIG__

#define CORE_NO_COPY(name) name(const name &) = delete; name& operator=(const name &) = delete;

#elif defined(COMPILER_GCC_COMPAT)
#define CORE_ALIGNOF(t) __alignof(t)
#define CORE_ALIGN(v) __attribute__((aligned(v)))
#define CORE_ALIGN_4 __attribute__((aligned(4)))
#define CORE_ALIGN_8 __attribute__((aligned(8)))
#define CORE_ALIGN_16 __attribute__((aligned(16)))

#define CORE_FORCE_INLINE inline __attribute__((always_inline))

#define CORE_NO_WARNING(b) core::internal::no_warning(b)
namespace core { namespace internal { template <typename T> CORE_FORCE_INLINE T no_warning(T x) { return x; } } }

#define CORE_BREAKPOINT __asm__("int $3")
#define CORE_STATIC_ASSERT_TEMPLATE(val, arg, param) \
    struct StaticAssertClass##__LINE__ { static void execute() { int a; { param* a; (void)a; } } enum { cVal }; }; \
    static_assert(((void)StaticAssertClass##__LINE__)::cVal, val), arg)
#define CORE_DECLARE_UNREACHABLE() __builtin_unreachable()
#define CORE_STATIC_ANALYSIS_ASSUME(exp)
#define CORE_STATIC_ANALYSIS_NON_NULL
#define CORE_NOEXCEPT noexcept
#define CORE_NO_INLINE __attribute__((noinline))
#define CORE_FUNC_NAME __PRETTY_FUNCTION__

#define CORE_NO_COPY(name) name(const name &); name& operator=(const name &);

#endif // COMPILER_GCC_COMPAT