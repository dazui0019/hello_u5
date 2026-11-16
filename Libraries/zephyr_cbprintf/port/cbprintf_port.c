#include "cbprintf.h"
#include "printf.h"

#ifdef _WIN32
#define CBPRINTF_PORT_WEAK
#else
#define CBPRINTF_PORT_WEAK __attribute__((weak))
#endif

CBPRINTF_PORT_WEAK __attribute__((noreturn)) void cbprintf_port_assert_fail(
    const char *expression, const char *file, unsigned int line)
{
    (void)expression;
    (void)file;
    (void)line;
    __builtin_trap();
}

CBPRINTF_PORT_WEAK void cbprintf_port_warn(const char *format, ...)
{
    /* Optional non-recursive diagnostics hook. */
    (void)format;
}

CBPRINTF_PORT_WEAK bool cbprintf_port_is_rodata(const void *pointer)
{
#if defined(__arm__) || defined(__thumb__)
    extern const unsigned char __cbprintf_rodata_start[];
    extern const unsigned char __cbprintf_rodata_end[];
    uintptr_t address = (uintptr_t)pointer;
    return address >= (uintptr_t)__cbprintf_rodata_start &&
           address < (uintptr_t)__cbprintf_rodata_end;
#else
    /* Host tests conservatively copy all string arguments. */
    (void)pointer;
    return false;
#endif
}

struct formatter_context {
    cbprintf_cb output;
    void *context;
    int error;
};

static void formatter_output(char character, void *argument)
{
    struct formatter_context *context = argument;
    if (context->error >= 0) {
        int result = context->output((unsigned char)character, context->context);
        if (result < 0) {
            context->error = result;
        }
    }
}

int z_cbvprintf_impl(cbprintf_cb out, void *ctx, const char *format,
                    va_list ap, uint32_t flags)
{
    if (flags != 0) {
        return -ENOTSUP;
    }
    if (out == NULL || format == NULL) {
        return -EINVAL;
    }
    struct formatter_context context = {out, ctx, 0};
    int result = vfctprintf(formatter_output, &context, format, ap);
    return context.error < 0 ? context.error : result;
}

int cbprintf(cbprintf_cb out, void *ctx, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int result = z_cbvprintf_impl(out, ctx, format, ap, 0);
    va_end(ap);
    return result;
}
