#ifndef LUMS_INCLUDED_IO_ABI_H
#define LUMS_INCLUDED_IO_ABI_H

#include <Lums/Util/Common.h>

#if defined(LUMS_BUILD_IO)
# define LUMS_API_IO LUMS_DLLEXPORT
#else
# define LUMS_API_IO LUMS_DLLIMPORT
#endif

#endif /* LUMS_INCLUDED_IO_ABI_H */
