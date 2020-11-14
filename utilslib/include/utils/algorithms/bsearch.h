#ifndef _UTILSLIB_BSEARCH_H
#define _UTILSLIB_BSEARCH_H

#include <cstdint>
#include <cstdarg>

#include "utils/utils_def.h"

typedef int32_t (CDECL BSearchCompare)(const void *elem1 REGISTER_PARAM(a0), const void *elem2 REGISTER_PARAM(a1));
C_FUNCTION bool binarySearch(const void *key REGISTER_PARAM(a0), const void *base REGISTER_PARAM(a1), size_t nelem REGISTER_PARAM(d0), size_t width REGISTER_PARAM(d1), void **pos REGISTER_PARAM(a2), BSearchCompare *fcmp REGISTER_PARAM(a3));

#endif // _UTILSLIB_BSEARCH_H
