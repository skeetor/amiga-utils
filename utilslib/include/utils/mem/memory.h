#ifndef _UTILSLIB_MEMORY_H
#define _UTILSLIB_MEMORY_H

#include "utils/utils_def.h"

/**
 * Copy N 32 Bit values
 */
C_FUNCTION void memcpyl(uint32_t items REGISTER_PARAM(d0), void *src REGISTER_PARAM(a0), void *dest REGISTER_PARAM(a1));

/**
 * Copy N 16 Bit values
 */
C_FUNCTION void memcpyw(uint32_t items REGISTER_PARAM(d0), void *src REGISTER_PARAM(a0), void *dest REGISTER_PARAM(a1));

/**
 * Copy 8 bit values until a 0-terminator is encountered.
 * The number of copied items (including the 0-terminator)
 * is returned.
 */
C_FUNCTION uint32_t zcopyb(void *src REGISTER_PARAM(a0), void *dest REGISTER_PARAM(a1));

/**
 * Copy 16 bit values until a 0-terminator is encountered.
 * The number of copied items (including the 0-terminator)
 * is returned.
 */
C_FUNCTION uint32_t zcopyw(void *src REGISTER_PARAM(a0), void *dest REGISTER_PARAM(a1));

/**
 * Copy 32 bit values until a 0-terminator is encountered.
 * The number of copied items (including the 0-terminator)
 * is returned.
 */
C_FUNCTION uint32_t zcopyl(void *src REGISTER_PARAM(a0), void *dest REGISTER_PARAM(a1));

#endif // _UTILSLIB_MEMORY_H
