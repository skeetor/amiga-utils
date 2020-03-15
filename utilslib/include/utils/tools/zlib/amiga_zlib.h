#ifndef _ZLIB_AMIGA_H
#define _ZLIB_AMIGA_H

#ifndef ZEXPORT
#define ZEXPORT __stdargs
#endif

#include "zlib.h"

#include <utils/utils_def.h>

/**
 ************************************************************
 * Return the maximum size the compressed buffer can
 * reach. This can be larger then the input buffer if
 * the source data is already compressed and can not be
 * compressed more.
 *
 * INPUT: Buffer length
 *
 * RETURN: Estimated maximum buffer length
 *
 ************************************************************
 */
C_FUNCTION uint32_t zlibMaxCompressedLen(uint32_t compressionLength REGISTER_PARAM(d0));

C_FUNCTION uint32_t zlibCompressData(const void *srcBuffer REGISTER_PARAM(a0), uint32_t nSrcLen REGISTER_PARAM(d0), void *destBuffer REGISTER_PARAM(a1), uint32_t destLen REGISTER_PARAM(d1));
C_FUNCTION uint32_t zlibUncompressData(const void *srcBuffer REGISTER_PARAM(a0), uint32_t nSrcLen REGISTER_PARAM(d0), void *destBuffer REGISTER_PARAM(a1), uint32_t destLen REGISTER_PARAM(d1));

#endif // _ZLIB_AMIGA_H
