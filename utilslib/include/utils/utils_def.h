#ifndef _UTILSLIB_DEF_H
#define _UTILSLIB_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#define C_FUNCTION	extern "C"
#else
#define C_FUNCTION
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#ifndef TO_STRING
#define __STR2__(x) #x
#define TO_STRING(x) __STR2__(x)
#endif

#ifndef REGISTER_PARAM
#define REGISTER_PARAM(x)	__asm(TO_STRING(x))
#endif

#endif // _UTILSLIB_DEF_H
