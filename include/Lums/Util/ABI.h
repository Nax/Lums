#ifndef LUMS_INCLUDED_ABI_H
#define LUMS_INCLUDED_ABI_H 1

#include <Lums/Util/Common.h>

#if defined(LUMS_BUILD)
# define LUMS_API LUMS_DLLEXPORT
#else
# define LUMS_API LUMS_DLLIMPORT
#endif

#endif
