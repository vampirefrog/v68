#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <portaudio.h>
#include "platform.h"

#ifdef __MINGW32__
#define NEED_COLOR_FIX
#endif

#ifdef NEED_COLOR_FIX

#include <windows.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

#endif

#include "v68.h"
#include "v68io.h"
#include "tools.h"

#define DEFAULT_CPU_CLOCK   8000000
#define DEFAULT_RAM_SIZE    8 * 1024 * 1024
#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_BUFFER_SIZE 4096

int opt_cpu_clock   = DEFAULT_CPU_CLOCK;
int opt_ram_size    = DEFAULT_RAM_SIZE;
int opt_sample_rate = DEFAULT_SAMPLE_RATE;
int opt_buffer_size = DEFAULT_BUFFER_SIZE;
char *opt_vgm_file  = 0;
int opt_utf8        = 0;
int opt_verbosity   = 0;
int opt_log_calls   = 0;
int opt_dasm        = 0;
char cmd_queue[V68_CMD_QUEUE_LEN][1024];
int cmd_queue_pos;

void queue_cmd(char *cmdline) {
	if(cmd_queue_pos > V68_CMD_QUEUE_LEN) return;

	strncpy(cmd_queue[cmd_queue_pos], cmdline, sizeof(cmd_queue[0]));
	cmd_queue_pos++;
}

/* TODO: parse k, M, G etc */
int parse_intval(char *s) {
	return atoi(s);
}

int running = 1;

void sighandler(int signum) {
	signal(signum, NULL);
	printf("Caught signal %d, coming out...\n", signum);
	running = 0;
}

void print_help(char *argv0) {
	printf("Usage: %s [options] command [args]\n", argv0);
	printf("Options:\n");
	printf("\t-c <command>      Execute this command before main command. Example: -c mxdrv.x.\n");
	printf("\t-C <clock>        CPU clock. Default is %dMHz.\n", DEFAULT_CPU_CLOCK / 1000000);
	printf("\t-M <size>         Specify size of RAM. Default is %dM.\n", DEFAULT_RAM_SIZE / 1024 / 1024);
	printf("\t-R <rate>         Sample rate. Default is %d.\n", DEFAULT_SAMPLE_RATE);
	printf("\t-b <size>         Audio buffer size. Default is %d.\n", DEFAULT_BUFFER_SIZE);
	printf("\t-l <logfile.vgm>  VGM file logging.\n");
	printf("\t-u                Convert output to utf-8.\n");
	printf("\t-v                Increase verbosity by 1.\n");
	printf("\t-t                Trace system calls (DOS, FE and IOCS).\n");
	printf("\t-d                Disassemble instructions as they are executed.\n");
}

int parse_cmdline(int argc, char **argv) {
	int first_nonarg_reached = 0;

	char cmdbuf[1024];
	cmdbuf[0] = 0;

	for(int i = 1; i < argc; i++) {
		if(!first_nonarg_reached) {
			if(!strcmp(argv[i], "-c")) {
				if(argc > i+1) {
					i++;
					queue_cmd(argv[i]);
				} else {
					fprintf(stderr, "-c requires an argument\n");
				}
			} else if (!strcmp(argv[i], "-C")) {
				i++;
				opt_cpu_clock = parse_intval(argv[i]);
			} else if (!strcmp(argv[i], "-M")) {
				i++;
				opt_ram_size = parse_intval(argv[i]);
			} else if (!strcmp(argv[i], "-R")) {
				i++;
				opt_sample_rate = parse_intval(argv[i]);
			} else if (!strcmp(argv[i], "-b")) {
				i++;
				opt_buffer_size = parse_intval(argv[i]);
			} else if (!strcmp(argv[i], "-l")) {
				i++;
				opt_vgm_file = strdup(argv[i]);
			} else if (!strcmp(argv[i], "-u")) {
				opt_utf8 = 1;
			} else if (!strcmp(argv[i], "-vvv")) {
				opt_verbosity+=3;
			} else if (!strcmp(argv[i], "-vv")) {
				opt_verbosity+=2;
			} else if (!strcmp(argv[i], "-v")) {
				opt_verbosity++;
			} else if (!strcmp(argv[i], "-t")) {
				opt_log_calls = 1;
			} else if (!strcmp(argv[i], "-d")) {
				opt_dasm = 1;
			} else if(argv[i][0] == '-') {
				print_help(argv[0]);
				return 1;
			} else {
				first_nonarg_reached = 1;
			}
		}

		if(first_nonarg_reached) {
			strncat(cmdbuf, argv[i], sizeof(cmdbuf) - strlen(cmdbuf));
			if(i + 1 < argc)
				strncat(cmdbuf, " ", sizeof(cmdbuf) - strlen(cmdbuf));
		}
	}
	if(cmdbuf[0]) {
		queue_cmd(cmdbuf);
		return 0;
	}

	print_help(argv[0]);
	return 1;
}

int main(int argc, char **argv, char **envp) {
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

	if(parse_cmdline(argc, argv)) {
		fprintf(stderr, "Could not parse command line\n");
		return -1;
	}

	int er = v68_init(opt_cpu_clock, opt_ram_size, opt_sample_rate);
	if(er) {
		fprintf(stderr, "Could not init VM! (%x)\n", er);
		return -1;
	}

	v68.log_calls = opt_log_calls;
	v68.log_dasm = opt_dasm;
	v68.verbosity = opt_verbosity;
	v68_boot();

	for(int i = 0; i < cmd_queue_pos; i++) {
		v68_queue_command(cmd_queue[i]);
	}
	v68_io_autodetect_drives();
	v68_dump_drives();
	char *v68_path = getenv("V68_PATH");
	v68_env_set("PATH", v68_path ? v68_path : getenv("PATH"));
	for(char **e = envp; *e; e++) {
		v68_env_append(*e);
	}

	v68_run();

	PaStream *stream;
	PaError err;

	if(v68.sound_touched) {
		printf("init sound\n");

		err = Pa_Initialize();
		if( err != paNoError ) goto error;

		err = Pa_OpenDefaultStream(
			&stream,
			0,              /* no input channels */
			2,              /* stereo output */
			paInt16,        /* 16 bit floating point output */
			opt_sample_rate,
			opt_buffer_size,    /* frames per buffer */
			NULL,
			NULL
		);
		if( err != paNoError ) goto error;

		err = Pa_StartStream( stream );
		if( err != paNoError ) goto error;

		signal(SIGINT, sighandler);

		if(opt_vgm_file && opt_vgm_file[0]) {
			struct vgm_logger l;
			vgm_logger_begin(&l, opt_vgm_file);
			v68.logger = &l;
		}

#define ALLOCBUF(b, s) \
	int16_t *b = malloc(s); \
	if(!b) { \
		fprintf(stderr, "Could not allocate %"PRIuSIZET" bytes sound bufer\n", s); \
		return 1; \
	}
		ALLOCBUF(bufL, opt_buffer_size * sizeof(*bufL));
		ALLOCBUF(bufR, opt_buffer_size * sizeof(*bufR));
		ALLOCBUF(tmpBufL, opt_buffer_size * sizeof(*tmpBufL));
		ALLOCBUF(tmpBufR, opt_buffer_size * sizeof(*tmpBufR));
		ALLOCBUF(buf, opt_buffer_size * 2 * sizeof(*buf));

		/* -- Play some stuff -- */
		while(running) {
			v68_fill_buffer(opt_buffer_size, bufL, bufR, tmpBufL, tmpBufR);
			for(int i = 0; i < opt_buffer_size; i++) {
				buf[i * 2] = bufL[i];
				buf[i * 2 + 1] = bufR[i];
			}
			err = Pa_WriteStream( stream, buf, opt_buffer_size);
			if( err ) goto error;
		}

		/* -- Close and shutdown -- */
		err = Pa_StopStream( stream );
		if( err != paNoError ) goto error;
		err = Pa_CloseStream( stream );
		if( err != paNoError ) goto error;
		Pa_Terminate();

		if(v68.logger)
			vgm_logger_end(v68.logger);
	}

	v68_shutdown();

	return err;

error:
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	return err;
}
