#include <stdio.h>

#ifdef __MINGW32__
#define NEED_COLOR_FIX
#endif

#ifdef NEED_COLOR_FIX

#include <windows.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

#endif

int main(int argc, char **argv) {
#ifdef NEED_COLOR_FIX
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle != INVALID_HANDLE_VALUE) {
      DWORD mode = 0;
        if (GetConsoleMode(handle, &mode)) {
          mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
          SetConsoleMode(handle, mode);
        }
    }
#endif

	printf("\033[33mANSI Color test\033[0m\n");

	return 0;
}
