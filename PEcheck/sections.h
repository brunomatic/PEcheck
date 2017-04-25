#ifndef SECTIONS_HEADER_H
#define SECTIONS_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include "pe_std.h"
#include "pe_opt.h"

typedef struct {
	uint8_t name[8];	// name of section
	uint32_t size;		// size of section - when loaded in memory
	uint32_t RVA;		// virtual adress of start of a section - relative to image base
	uint32_t size_raw;	// raw section size - on disk
	uint32_t ptr_data;	// pointer to start of section in raw offset
	uint32_t ptr_relocs;// pointer to relocation table
	uint32_t ptr_line;	// pointer to line numbers ?
	uint16_t num_relocs;// number or relocations in table
	uint16_t num_line;	// number of line numbers ?
	uint32_t characteristics; // characteristics flags
} SECTION_HEADER;


int read_section_table(FILE *file, SECTION_HEADER ** headers, uint16_t num_sections, uint32_t offset);

int find_idata_section(PE_STANDARD_HEADER * std_hdr, PE_OPTIONAL_HEADER * opt_hdr, SECTION_HEADER * section_hdrs);

#endif // ! SECTIONS_HEADER
