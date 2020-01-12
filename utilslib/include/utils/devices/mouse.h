#ifndef _UTILS_MOUSE_I
#define _UTILS_MOUSE_I

#include "utils/utils_def.h"

/**
 * Check if the left Mousebutton has been pressed. If yes,
 * it waits until it is released again. If not, the function returns
 * immediately false.
 * If the button was pressed, after it is released, true is returned.
 */
C_FUNCTION bool CheckLeftMouseRelease(void);

/**
 * Check if the right Mousebutton has been pressed. If yes,
 * it waits until it is released again. If not, the function returns
 * immediately false.
 * If the button was pressed, after it is released, true is returned.
 */
C_FUNCTION bool CheckRightMouseRelease(void);

#endif // _UTILS_MOUSE_I
