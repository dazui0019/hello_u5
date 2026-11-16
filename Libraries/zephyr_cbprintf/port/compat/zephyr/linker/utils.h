#ifndef CBPRINTF_COMPAT_LINKER_UTILS_H
#define CBPRINTF_COMPAT_LINKER_UTILS_H
#include "cbprintf_port.h"
#define linker_is_in_rodata(pointer) cbprintf_port_is_rodata(pointer)
#endif
