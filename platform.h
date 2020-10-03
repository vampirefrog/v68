#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <inttypes.h>

#ifdef _WIN32
#  ifdef _WIN64
#    define PRIuSIZET PRIu64
#    define PRIxSIZET PRIu64
#    define PRIdSSIZET PRId64
#  else
#    define PRIuSIZET PRIu32
#    define PRIxSIZET PRIu32
#    define PRIdSSIZET PRId32
#  endif
#else
#  define PRIuSIZET "zu"
#  define PRIxSIZET "zx"
#  define PRIdSSIZET "zd"
#endif

#endif /* PLATFORM_H_ */
