#ifndef _UTILSLIB_MEMORY_H
#define _UTILSLIB_MEMORY_H

#include "utils/utils_def.h"

/**
 * Copy N 32 Bit values
 */
C_FUNCTION void memcpyl(uint32_t items, void *src, void *dest);

/**
 * Copy N 16 Bit values
 */
C_FUNCTION void memcpyw(uint32_t items, void *src, void *dest);

/**
 * Copy 8 bit values until a 0-terminator is encountered.
 * The number of copied items (including the 0-terminator)
 * is returned.
 */
C_FUNCTION uint32_t zcopyb(void *src, void *dest);

/**
 * Copy 16 bit values until a 0-terminator is encountered.
 * The number of copied items (including the 0-terminator)
 * is returned.
 */
C_FUNCTION uint32_t zcopyw(void *src, void *dest);
/**
 * Copy 32 bit values until a 0-terminator is encountered.
 * The number of copied items (including the 0-terminator)
 * is returned.
 */
C_FUNCTION uint32_t zcopyl(void *src, void *dest);

#endif // _UTILSLIB_MEMORY_H
