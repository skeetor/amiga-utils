#ifndef _UTILSLIB_DEBUG_H
#define _UTILSLIB_DEBUG_H

#include "utils/utils_def.h"

/**
 * Prints a message to current stdout and waits for a left mouse press.
 */
C_FUNCTION void DebugWait(const char *message REGISTER_PARAM(a0));

/**
 * Print the CPU Registers to stdout.
 */
C_FUNCTION void PrintCPU(void);

#endif // _UTILSLIB_DEBUG_H
