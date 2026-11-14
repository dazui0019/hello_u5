#ifndef CBPRINTF_COMPAT_LOGGING_LOG_H
#define CBPRINTF_COMPAT_LOGGING_LOG_H
#include "cbprintf_port.h"
#define LOG_MODULE_REGISTER(...)
#define LOG_WRN(...) cbprintf_port_warn(__VA_ARGS__)
#endif
