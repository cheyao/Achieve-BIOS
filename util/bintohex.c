#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// A short program to convert a binary file to hex
// (To convert the RISC-V object file use objconv)
// This file doesn't have any copyright with it
// (You can use it as you want)

int main(int argc, char **argv) {
	if (argc < 4) {
		fprintf(stderr, "Too few arguments provided!\nUsage: bintohex input_file output_file memory_size (hex)");
		return 1;
	}
	FILE *i = fopen(argv[1], "rb");
	FILE *o = fopen(argv[2], "w");
	uint64_t size = strtol(argv[3], NULL, 16);

	uint64_t t = 0;
	long long unsigned int k;
	while (1) {
		if (fread(&k, sizeof k, 1, i) != 1)
			break;

		fprintf(o, "%016llx\n", k);

		t++;
	}

	for (; t < size; t++)
		fputs("0000000000000000\n", o);

	fclose(o);
	fclose(i);

	return 0;
}
