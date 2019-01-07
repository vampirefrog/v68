#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <ao/ao.h>

#include "v68.h"
#include "v68io.h"
#include "tools.h"

#define BUF_SIZE 4096
#define SAMPLE_RATE 44100
#define V68_CLOCK 16000000
#define V68_RAM 2*1024*1024

int opt_utf8 = 0;
int opt_verbosity = 0;
int opt_log_calls = 0;
int opt_dasm = 0;

int running = 1;

void sighandler(int signum) {
	signal(signum, NULL);
	printf("Caught signal %d, coming out...\n", signum);
	running = 0;
}

void print_help(char *argv0) {
	printf("Usage: %s [options] command [args]\n", argv0);
	printf("Options:\n");
	printf("\t-c \"<command>\"  Execute this command before main command. Example: -c mxdrv.x.\n");
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
					v68_queue_command(argv[i]);
				} else {
					fprintf(stderr, "-c requires an argument\n");
				}
			} else if (!strcmp(argv[i], "-u")) {
				opt_utf8 = 1;
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
		v68_queue_command(cmdbuf);
		return 0;
	}

	print_help(argv[0]);
	return 1;
}

int main(int argc, char **argv, char **envp) {
	int er = v68_init(V68_CLOCK, V68_RAM, SAMPLE_RATE);
	if(er) {
		fprintf(stderr, "Could not init VM! (%x)\n", er);
		return -1;
	}

	if(parse_cmdline(argc, argv)) {
		fprintf(stderr, "Could not parse command line\n");
		return -1;
	}

	v68.log_calls = opt_log_calls;
	v68.log_dasm = opt_dasm;
	v68.verbosity = opt_verbosity;
	v68_io_autodetect_drives();
	v68_dump_drives();
	struct vgm_logger l;
	vgm_logger_begin(&l, "v68.vgm");
	v68.logger = &l;

	char *v68_path = getenv("V68_PATH");
	v68_env_set("PATH", v68_path ? v68_path : getenv("PATH"));
	for(char **e = envp; *e; e++) {
		v68_env_append(*e);
	}

	// AO
	ao_initialize();

	int default_driver = ao_default_driver_id();

	ao_sample_format format;
	memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 2;
	format.rate = SAMPLE_RATE;
	format.byte_format = AO_FMT_LITTLE;

	ao_device *device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}

	signal(SIGINT, sighandler);

	v68_queue_next_command();

	/* -- Play some stuff -- */
	while(running) {
		int16_t bufL[BUF_SIZE], bufR[BUF_SIZE];
		int16_t buf[BUF_SIZE * 2];
		v68_fill_buffer(bufL, bufR, BUF_SIZE);
		for(int i = 0; i < BUF_SIZE; i++) {
			buf[i * 2] = bufL[i];
			buf[i * 2 + 1] = bufR[i];
		}
		ao_play(device, (char *)buf, BUF_SIZE * format.channels * format.bits / 8);
	}

	/* -- Close and shutdown -- */
	ao_close(device);

	ao_shutdown();

	vgm_logger_end(&l);

	v68_shutdown();

	return 0;
}
