/* strsplit.c - splitting a string */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
strsplit(char *str, char *parts[], int nparts)
{
	char *tok;
	char *save = NULL;
	int i;

	for (i = 0; i < nparts; i++, str = NULL) {
		tok = strtok_r(str, " ", &save);
		if (tok == NULL)
			break;
		parts[i] = tok;
	}
	if (i == nparts)
		return -1;

	return i;
}

int
main(int argc, char *argv[])
{
	char *parts[128];
	int nparts;
	int i;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s STRING\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	if ((nparts = strsplit(argv[1], parts, 128)) < 0) {
		fprintf(stderr, "strsplit failed\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < nparts; i++)
		printf("%03d: %s\n", i, parts[i]);

	exit(EXIT_SUCCESS);
}
