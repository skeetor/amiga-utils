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

/**
 * Fill the specified memory with a given byte. Semantics is the same
 * as the C function memset but the name is different to avoid name
 * clashes when using C libraries.
 */
C_FUNCTION void memfill(void *ptr REGISTER_PARAM(a2), uint8_t value REGISTER_PARAM(d1), uint32_t length REGISTER_PARAM(d2));

/**
 * Fill the specified memory with a given byte using the blitter.
 * Blitter DMA has to be enabled before a call, otherwise it will
 * do nothing. For small sizes, the blitter is not used.
 * Since the blitter is used for bigger buffers, the buffersize may
 * not be larger then 128K. Alignment to even adresses is not needed
 * as this is automatically resolved.
 *
 * If bBlitterWait is false, then the function will start the blitter
 * and return immediatly, otherwise it will wait until the blitter is done.
 * In this case, the caller has to wait on his own and must ensure that the
 * blitter is not busy before a call.
 */
C_FUNCTION void memfillBlit(void *ptr REGISTER_PARAM(a2), uint8_t value REGISTER_PARAM(d1), uint32_t length REGISTER_PARAM(d2), bool bBlitterWait REGISTER_PARAM(d3));

#endif // _UTILSLIB_MEMORY_H
