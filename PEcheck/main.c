#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "ms_dos.h"
#include "pe_std.h"
#include "pe_opt.h"


int main(int argc, char **argv) {

	FILE * file;
	MS_DOS_HEADER ms_dos_header;
	uint32_t signature;
	PE_STANDARD_HEADER pe_standard_header;
	PE_OPTIONAL_HEADER pe_optional_header;


	if (argc != 2) {
		printf("Usage: pecheck.exe <name of PE file>\n");
		exit(EXIT_FAILURE);
	}

	file = fopen(argv[1], "rb");
	printf("Opening file: %s \n", argv[1]);

	if (!file) {
		printf("Can't open file! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}



	if (!read_ms_dos_header(file, &ms_dos_header)) {
		printf("Error while reading MS-DOS header! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	print_ms_dos_header(&ms_dos_header);

	if (!read_pe_header(file, &pe_standard_header, ms_dos_header.pe_header_offset)) {
		printf("Error while reading PE header or signature! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	print_pe_std_header(&pe_standard_header);


	if (!read_optional_header(file, &pe_optional_header, (ms_dos_header.pe_header_offset + sizeof(PE_STANDARD_HEADER) + 4))) {
		printf("Error while reading PE optional header or 64-bit PE file! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


	print_pe_opt_header(&pe_optional_header);

	printf("Closing file: %s \n", argv[1]);
	fclose(file);

	getchar();
	return 0;
}