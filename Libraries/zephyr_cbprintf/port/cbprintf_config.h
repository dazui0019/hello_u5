#ifndef CBPRINTF_CONFIG_H
#define CBPRINTF_CONFIG_H

/* Disabled Zephyr CONFIG_* options must stay undefined (several use #ifdef). */
#define CONFIG_CBPRINTF_STATIC_PACKAGE_CHECK_ALIGNMENT 1
#define CONFIG_CBPRINTF_CONVERT_CHECK_PTR 1

/* Match the application's Debug -O0 build; avoids five temporary arrays. */
#ifndef __OPTIMIZE__
#define CONFIG_NO_OPTIMIZATIONS 1
#endif

/* The current printf formatter accepts ordinary, untagged arguments only. */
#if defined(CONFIG_CBPRINTF_PACKAGE_SUPPORT_TAGGED_ARGUMENTS) || \
    defined(CONFIG_CBPRINTF_PACKAGE_LONGDOUBLE) || defined(CONFIG_CBPRINTF_LIBC_SUBSTS)
#error "Tagged arguments, long double and libc substitution are not enabled in this port"
#endif

#endif
