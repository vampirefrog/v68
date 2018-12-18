#include <limits.h>

#include "v68io.h"

int main(int argc, char **argv, char **envp) {
	v68_io_init();
	v68_io_autodetect_drives();
	v68_dump_drives();

	char buf[PATH_MAX];
	char *c = v68_io_xlate_path("/media/tb/vgm/ProjectAY.zip", buf, PATH_MAX);
	printf("%s\n", c);

	c = v68_io_xlate_dos_path("h:\\foo.txt", buf, PATH_MAX);
	printf("%s\n", c);

	c = v68_io_xlate_dos_path("D:\\Program%20Files%20(x86)\\dbForGe%20Studio%20for%20MySQL\\dbForge%20Studio%20for%20MySQL.exe", buf, PATH_MAX);
	printf("%s\n", c);

	char buf2[PATH_MAX];
	c = v68_io_resolve_path(buf, buf2, PATH_MAX);
	printf("resolved %s -> %s\n", buf, c);
	return 0;
}
