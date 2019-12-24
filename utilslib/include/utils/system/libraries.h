#ifndef _UTILSLIB_LIBRARIES_I
#define _UTILSLIB_LIBRARIES_I 1

#include <utils/utils_def.h>

/**
 * Open-/CloseDOSLibrary
 */
C_FUNCTION DOSLibrary *OpenDOSLibray(uint32_t nVersion = 0);
C_FUNCTION void CloseDOSLibray(DOSLibrary *library);

/**
 * Open-/CloseGraphicsLibrary
 */
C_FUNCTION GraphicsLibrary *OpenGraphicsLibray(uint32_t nVersion = 0);
C_FUNCTION void CloseGraphicsLibray(GraphicsLibrary *library);

#endif // _UTILSLIB_LIBRARIES_I
