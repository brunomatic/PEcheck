#include <stdio.h>
#include <stdlib.h>
#include "sections.h"
#include "pe_opt.h"
#include "pe_std.h"


int read_section_table(FILE *file, SECTION_HEADER ** headers, uint8_t num_sections, uint32_t offset) {
	SECTION_HEADER * section_hdr;
	int i;
	
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	section_hdr = malloc(sizeof(SECTION_HEADER)*num_sections);

	if (!fread(section_hdr, sizeof(SECTION_HEADER), num_sections, file)) {
		return 0;
	}

	*headers = section_hdr;

	return 1;
}

int find_import_section(PE_STANDARD_HEADER * std_hdr, PE_OPTIONAL_HEADER * opt_hdr, SECTION_HEADER * section_hdrs) {
	int i;

	/*
		Check each section header if it contains import table RVA, if it does it contains import data
	*/

	for (i = 0; i < std_hdr->num_of_sections; i++) {
		
		if ((opt_hdr->data_dirs[1].RVA < (section_hdrs[i].VA + section_hdrs[i].size)) && (opt_hdr->data_dirs[1].RVA > section_hdrs[i].VA))
			return i;

	}
	return 0;
}