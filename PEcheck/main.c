#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "ms_dos.h"
#include "pe_std.h"
#include "pe_opt.h"
#include "sections.h"


int main(int argc, char **argv) {

	uint32_t offset;
	FILE * file;
	MS_DOS_HEADER ms_dos_header;
	uint32_t signature;
	PE_STANDARD_HEADER pe_standard_header;
	PE_OPTIONAL_HEADER pe_optional_header;
	SECTION_HEADER * section_headers;
	idata_section import_section;
	int import_header_index;


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

	/*
		Read and print MZ header
	*/

	if (!read_ms_dos_header(file, &ms_dos_header)) {
		printf("Error while reading MS-DOS header! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	print_ms_dos_header(&ms_dos_header);


	/*
		Read and print PE standard header
	*/
	offset = ms_dos_header.pe_header_offset;
	if (!read_pe_header(file, &pe_standard_header, offset)) {
		printf("Error while reading PE header or signature! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	print_pe_std_header(&pe_standard_header);


	/*
		Read and print optional header
	*/
	offset = (ms_dos_header.pe_header_offset + sizeof(PE_STANDARD_HEADER));
	if (!read_optional_header(file, &pe_optional_header, offset)) {
		printf("Error while reading PE optional header or 64-bit PE file! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	print_pe_opt_header(&pe_optional_header);


	/*
		Read section table
	*/
	offset = (ms_dos_header.pe_header_offset + sizeof(PE_STANDARD_HEADER) + pe_standard_header.size_of_opt_header);
	if (!read_section_table(file, &section_headers, pe_standard_header.num_of_sections, offset )) {
		printf("Error while reading section table! Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("Closing file: %s \n", argv[1]);
	fclose(file);

	getchar();
	exit(EXIT_SUCCESS);
}