#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

// A short program to convert a binary file to hex
// (To convert the RISC-V object file use objconv)
// This file doesn't have any copyright with it
// (You can use it as you want)

int main(int argc, char **argv) {
	if (argc <= 4) {
		fprintf(stderr, "Too few arguments provided!\nUsage: bintohex input_file output_file memory_size (hex)");
	}
	FILE *i = fopen(argv[2], "rb"), *o = fopen(argv[2], "rb");
	uint64_t size = strtol(argv[3], NULL, 16);

	uint64_t t;
	uint64_t i;
	while (1) {
		if (fread(&i, sizeof i, 1, i) != 1)
			break;

		fprintf(o, "%08x ", i);

		t++;
	}

	for (; t < size; t++) {
		fprintf(o, "00000000000000000000000000000000 ", i);
	}

	return 0;
}
