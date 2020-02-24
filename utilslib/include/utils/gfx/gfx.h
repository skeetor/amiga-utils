#ifndef _UTILSLIB_GFX_I
#define _UTILSLIB_GFX_I 1

#include <utils/utils_def.h>

/**
 * Open-/CloseGraphicsLibrary
 */
C_FUNCTION GraphicsLibrary *OpenGraphicsLibray(uint32_t nVersion REGISTER_PARAM(d0) = 0);
C_FUNCTION void CloseGraphicsLibray(void);

#endif // _UTILSLIB_GFX_I
