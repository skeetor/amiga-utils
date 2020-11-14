#ifndef _UTILSLIB_DEF_H
#define _UTILSLIB_DEF_H

#include <stdint.h>
#include <stddef.h>

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

#ifndef STDARGS
	#ifdef AMIGA_OS
		#define STDARGS __stdargs
	#else
		#define STDARGS
	#endif
#endif // STDARGS

#ifndef CDECL
	#ifdef AMIGA_OS
		#define CDECL STDARGS
	#elif defined(_MSC_VER)
		#define CDECL __cdecl
	#else
		#define CDECL
	#endif
#endif // CDECL

#ifndef REGISTER_PARAM
	#ifdef AMIGA_OS
		#define REGISTER_PARAM(x)	__asm(TO_STRING(x))
	#else
		#define REGISTER_PARAM(x)
	#endif
#endif

#ifndef REGPARAM
	#ifdef _MSC_VER
		#define REGPARAM(x)
	#else
		#define REGPARAM(x) __attribute__ ((regparm(x)))
	#endif
#endif

#define invalid_t -1

#endif // _UTILSLIB_DEF_H
