#ifndef LUMS_INCLUDED_DRAW_ABI_H
#define LUMS_INCLUDED_DRAW_ABI_H

#include <Lums/Util/Common.h>

#if defined(LUMS_BUILD_DRAW)
# define LUMS_API_DRAW LUMS_DLLEXPORT
#else
# define LUMS_API_DRAW LUMS_DLLIMPORT
#endif

#endif /* LUMS_INCLUDED_DRAW_ABI_H */
