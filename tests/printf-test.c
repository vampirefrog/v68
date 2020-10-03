#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef _WIN32
#  ifdef _WIN64
#    define PRI_SIZET PRIu64
#    define PRI_SSIZET PRId64
#  else
#    define PRI_SIZET PRIu32
#    define PRI_SSIZET PRId32
#  endif
#else
#  define PRI_SIZET "zu"
#  define PRI_SSIZET "zd"
#endif

int main(int argc, char **argv) {
	uint8_t         u8 = __LINE__; printf(" uint8_t       %"PRIu8"\n",     u8 );
	int8_t          i8 = __LINE__; printf("  int8_t       %"PRId8"\n",     i8 );
	uint16_t       u16 = __LINE__; printf("uint16_t       %"PRIu16"\n",    u16);
	int16_t        i16 = __LINE__; printf(" int16_t       %"PRId16"\n",    i16);
	uint32_t       u32 = __LINE__; printf("uint32_t       %"PRIu32"\n",    u32);
	int32_t        i32 = __LINE__; printf(" int32_t       %"PRId32"\n",    i32);
	uint64_t       u64 = __LINE__; printf("uint64_t       %"PRIu64"\n",    u64);
	int64_t        i64 = __LINE__; printf(" int64_t       %"PRId64"\n",    i64);
	intmax_t         m = __LINE__; printf("intmax_t       %"PRIdMAX"\n",    m );
	uintmax_t       um = __LINE__; printf("uintmax_t      %"PRIuMAX"\n",    um);
	size_t           s = __LINE__; printf("size_t         %"PRI_SIZET"\n",   s);
	ssize_t         ss = __LINE__; printf("ssize_t        %"PRI_SSIZET"\n", ss);
	char             c = __LINE__; printf("char           %d\n",  c  );
	unsigned char    u = __LINE__; printf("unsigned char  %d\n",  u  );
	short           sh = __LINE__; printf("short          %d\n",  sh );
	unsigned short ush = __LINE__; printf("unsigned short %d\n",  ush);
	long             l = __LINE__; printf("long           %ld\n", l  );
	unsigned long   ul = __LINE__; printf("unsigned long  %lu\n", ul );
	int              i = __LINE__; printf("int            %d\n",  i  );
	unsigned int    ui = __LINE__; printf("unsigned int   %d\n",  ui );

	return 0;
}
