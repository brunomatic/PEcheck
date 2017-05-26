#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include "ms_dos.h"
#include "pe_std.h"
#include "pe_opt.h"
#include "sections.h"
#include "idata_section.h"
#include "edata_section.h"


int main(int argc, char **argv) {

	int32_t offset;
	int32_t temp;
	FILE * file;
	MS_DOS_HEADER ms_dos_header;
	PE_STANDARD_HEADER pe_standard_header;
	PE_OPTIONAL_HEADER pe_optional_header;
	SECTION_HEADER * section_headers;
	int header_index;
	IMPORT_DIRECTORY_TABLE idata_directory_table;
	EDATA_SECTION  * edata_table;


	if (argc != 2) {
		printf("Usage: pecheck.exe <name of PE file>\n");
		exit(EXIT_FAILURE);
	}

	file = fopen(argv[1], "rb");
	printf("Opening file: %s \n", argv[1]);

	if (!file) {
		printf("Can't open file!\n");
		exit(EXIT_FAILURE);
	}

	/*
		Read and print MZ header
	*/

	if (!read_ms_dos_header(file, &ms_dos_header)) {
		printf("Error while reading MS-DOS header!\n");
		exit(EXIT_FAILURE);
	}

	print_ms_dos_header(&ms_dos_header);


	/*
		Read and print PE standard header
	*/
	offset = ms_dos_header.pe_header_offset;
	if (!read_pe_header(file, &pe_standard_header, offset)) {
		printf("Error while reading PE header or signature!\n");
		exit(EXIT_FAILURE);
	}

	print_pe_std_header(&pe_standard_header);


	/*
		Read and print optional header
	*/
	offset = (ms_dos_header.pe_header_offset + sizeof(PE_STANDARD_HEADER));
	if (!read_optional_header(file, &pe_optional_header, offset)) {
		printf("Error while reading PE optional header or 64-bit PE file!\n");
		exit(EXIT_FAILURE);
	}

	print_optional_header(&pe_optional_header);


	/*
		Read section table
	*/
	offset = (ms_dos_header.pe_header_offset + sizeof(PE_STANDARD_HEADER) + pe_standard_header.size_of_opt_header);
	if (!read_section_table(file, &section_headers, pe_standard_header.num_of_sections, offset )) {
		printf("Error while reading section table!\n");
		exit(EXIT_FAILURE);
	}

	print_sections(&pe_standard_header, section_headers);


	header_index = find_idata_section(&pe_standard_header, &pe_optional_header, section_headers);
	
	if (header_index == -1) {
		printf("===========================\n");
		printf("idata section not found!\n");
		printf("===========================\n");
	}
	else {
		
		// directory table offset = RVA(import table) - RVA(idata_section) + ptr_to_raw_data(idata_section)
		// number of entries = size(import table) / sizeof (data directory entry) - 1
		temp = (pe_optional_header.data_dirs[1].size / DIR_ENTRY_SIZE) - 1 ;
		offset = read_idata_directory_table(file, &idata_directory_table, pe_optional_header.data_dirs[1].RVA, section_headers[header_index].RVA, section_headers[header_index].ptr_data, temp);
		if (!offset) {
			printf("Error while reading .idata section!\n");
			exit(EXIT_FAILURE);
		}
		
		// print this nightmare 
		offset = section_headers[header_index].ptr_data - section_headers[header_index].RVA;
		print_idata_directory_table(&idata_directory_table, offset);

	}

	header_index = find_edata_section(&pe_standard_header, &pe_optional_header, section_headers);

	if (header_index == -1) {
		printf("===========================\n");
		printf("edata section not found!\n");
		printf("===========================\n");
	}
	else {

		offset = read_edata_directory_table(file, &edata_table, pe_optional_header.data_dirs[0].RVA, section_headers[header_index].RVA, section_headers[header_index].ptr_data);
		if (!offset) {
			printf("Error while reading .edata section!\n");
			exit(EXIT_FAILURE);
		}

		// print this nightmare 
		offset = section_headers[header_index].ptr_data - section_headers[header_index].RVA;
		print_edata_directory_table(edata_table, offset);

		free(edata_table);

	}

	printf("Closing file: %s \n", argv[1]);
	fclose(file);

	getchar();
	exit(EXIT_SUCCESS);
}