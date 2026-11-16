#ifndef CBPRINTF_PORT_H
#define CBPRINTF_PORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>
#include "cbprintf_config.h"
#include "cbprintf_port_macros.h"

#if !defined(__GNUC__) || defined(__cplusplus)
#error "cbprintf port requires GNU C11 (GCC or Clang)"
#endif

#if UINTPTR_MAX > UINT32_MAX
#define CONFIG_64BIT 1
#endif

#ifndef __packed
#define __packed __attribute__((packed))
#endif
#ifndef __printf_like
#define __printf_like(fmt, args) __attribute__((format(__printf__, fmt, args)))
#endif
#ifndef __fallthrough
#define __fallthrough __attribute__((fallthrough))
#endif

#ifndef BIT
#define BIT(n) (1UL << (n))
#endif
#ifndef BIT_MASK
#define BIT_MASK(n) ((n) == 0 ? 0UL : (~0UL >> (sizeof(unsigned long) * 8 - (n))))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif
#ifndef ROUND_UP
#define ROUND_UP(v, a) (((v) + (a) - 1) & ~((__typeof__(v))(a) - 1))
#endif
#define Z_IS_POW2(v) ((v) && !((v) & ((v) - 1)))
#define Z_POW2_CEIL(v) ((v) <= 1 ? 1ULL : (1ULL << (64 - __builtin_clzll((v) - 1))))
#define BUILD_ASSERT(cond, ...) _Static_assert((cond), #cond)

__attribute__((noreturn)) void cbprintf_port_assert_fail(
    const char *expression, const char *file, unsigned int line);
void cbprintf_port_warn(const char *format, ...) __printf_like(1, 2);
bool cbprintf_port_is_rodata(const void *pointer);

/* Do not call LOG_* from these hooks: the logger may use this packager. */
#define __ASSERT(cond, ...) do { \
    if (!(cond)) cbprintf_port_assert_fail(#cond, __FILE__, __LINE__); \
} while (0)
#define __ASSERT_NO_MSG(cond) __ASSERT(cond)

static inline bool is_null_no_warn(const void *pointer)
{
    return pointer == NULL;
}

#endif
