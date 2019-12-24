#ifndef _UTILSLIB_MOUSHELPER_H
#define _UTILSLIB_MOUSHELPER_H

#include "utils/utils_def.h"

/**
 * Test if the right mousebutton has been pressed. If not
 * then 0 is returned. If yes, it waits until the button has been
 * released and 1 is returned
 */
C_FUNCTION bool WaitRightMouseRelease(void);

/**
 * Test if the left mousebutton has been pressed. If not
 * then 0 is returned. If yes, it waits until the button has been
 * released and 1 is returned
 */
C_FUNCTION bool WaitLeftMouseRelease(void);

#endif // _UTILSLIB_MOUSHELPER_H
