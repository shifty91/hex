/*
 * hex.c -- simple hexdump utility
 * Copyright (C) 2015 Kurt Kanzenbach <kurt@kmk-computers.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

/* adjust for your needs */
#define LINE_LENGTH 0x10

static struct option long_opts[] = {
	{ "include", no_argument, NULL, 'i' },
	{ NULL,      0,           NULL,  0  }
};

static
int process_line(const unsigned char *buf, unsigned int real_length)
{
	static unsigned int line_counter = 0;

	printf("%08x: ", line_counter);

	for (int i = 0; i < real_length; ++i) {
		printf("%02x", buf[i]);
		if ((i + 1) % 2 == 0)
			printf(" ");
	}
	for (int i = real_length; i < LINE_LENGTH; ++i) {
		if ((i + 1) % 2 == 0)
			printf("   ");
		else
			printf("  ");
	}

	printf(" |");
	for (int i = 0; i < real_length; ++i) {
		int c = (int)buf[i];
		if (c < 0x20 || c >= 0x7f)
			c = '.';
		printf("%c", c);
	}
	for (int i = real_length; i < LINE_LENGTH; ++i)
		printf(" ");
	printf("|\n");

	fflush(stdout);
	line_counter += LINE_LENGTH;
	return 0;
}

static int hexdump_file(FILE *f)
{
	unsigned int written = 0;
	unsigned char buf[LINE_LENGTH];
	int c;

	while ((c = fgetc(f)) != EOF) {
		buf[written++] = c;
		if (written == LINE_LENGTH) {
			written = 0;
			process_line(buf, LINE_LENGTH);
		}
	}
	if (ferror(f)) {
		perror("fgetc() failed");
		return -EIO;
	}

	if (written != 0)
		process_line(buf, written);

	return 0;
}

static void prepare_file_name(const char * restrict src, char * restrict dest)
{
	const char *p = src;
	char *s = dest;

	/* caller has to make sure, that dest is large enough :P */
	for (; *p ; p++, s++) {
		*s = *p;
		if (*p == '/' || *p == '.')
			*s = '_';
	}
	*s = '\0';
}

static int include_file(const char *file, FILE *f)
{
	char new_file[strlen(file) + 1];
	size_t bytes = 0, off = 0;
	int c;

	prepare_file_name(file, new_file);

	printf("unsigned char %s[] = {\n  ", new_file);

	while ((c = fgetc(f)) != EOF) {
		if (bytes != 0)
			printf(", ");

		if (off == LINE_LENGTH) {
			printf("\n  ");
			off = 0;
		}

		printf("0x%02x", (unsigned char)c);
		++bytes;
		++off;
	}
	printf("\n};\n");
	printf("unsigned int %s_len = %zu;\n", new_file, bytes);

	if (ferror(f)) {
		perror("fgetc() failed");
		return -EIO;
	}

	return 0;
}

int main(int argc, char **argv)
{
	FILE *f;
	int i = 0, c;

	while ((c = getopt_long(argc, argv, "i", long_opts, NULL)) != -1) {
		switch (c) {
		case 'i':
			i = 1;
			break;
		default:
			fprintf(stderr, "usage: hex [options] [file]\n");
			fprintf(stderr, "options:\n");
			fprintf(stderr, "  -i: generate C include file\n");
			fprintf(stderr, "Copyright (C) 2015 Kurt Kanzenbach <kurt@kmk-computers.de>\n");
			exit(EXIT_FAILURE);
		}
	}
	argc -= optind;
	argv += optind;

	f = argc > 0 ? fopen(argv[0], "r") : stdin;
	if (!f) {
		perror("fopen() failed");
		return EXIT_FAILURE;
	}

	if (i) {
		if (include_file(argc > 0 ? argv[0] : "stdin", f))
			return EXIT_FAILURE;
	} else {
		if (hexdump_file(f))
			return EXIT_FAILURE;
	}

	if (argc > 0)
		fclose(f);

	return EXIT_SUCCESS;
}
