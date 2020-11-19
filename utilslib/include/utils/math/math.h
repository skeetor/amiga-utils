#ifndef _UTILSLIB_MATH_H
#define _UTILSLIB_MATH_H

#include "utils/utils_def.h"

/**
 * Initialize the rnd() function with a random seed
 */
C_FUNCTION uint32_t rndInit(void);

/**
 * Set a seed. This should never be 0. A big prime works best.
 */
C_FUNCTION void rndSetSeed(uint32_t seed REGISTER_PARAM(d0));

/**
 * Get the starting seed which started the current sequence.
 */
C_FUNCTION uint32_t rndGetSeed(void);

/**
 * Return a random 32 bit value
 */
C_FUNCTION uint32_t random(void);

#endif // _UTILSLIB_MATH_H
