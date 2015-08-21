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

/* adjust for your needs */
static const unsigned int LINE_LENGTH = 0x10;

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
		if (c <= 0x20 || c >= 0x7f)
			c = '.';
		printf("%c", c);
	}
	printf("|\n");

	fflush(stdout);
	line_counter += LINE_LENGTH;
	return 0;
}

int main(int argc, char **argv)
{
	unsigned int written = 0;
	unsigned char buf[LINE_LENGTH];
	int c;

	while ((c = getc(stdin)) != EOF) {
		buf[written++] = c;
		if (written == LINE_LENGTH) {
			written = 0;
			process_line(buf, LINE_LENGTH);
		}
	}

	if (written != 0)
		process_line(buf, written);

	return EXIT_SUCCESS;
}
