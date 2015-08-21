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
