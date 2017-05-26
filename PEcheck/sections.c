#include <stdio.h>
#include <stdlib.h>
#include "sections.h"
#include "pe_opt.h"
#include "pe_std.h"


int read_section_table(FILE *file, SECTION_HEADER ** headers, uint16_t num_sections, uint32_t offset) {
	SECTION_HEADER * section_hdr;
	
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

void print_sections(PE_STANDARD_HEADER * std_hdr, SECTION_HEADER * section_hdrs) {

	int i;

	printf("====== Sections headers ======\n");

	for (i = 0; i < std_hdr->num_of_sections; i++) {

		printf("----------------------------------------- \n");
		printf("%s \n", (char *)section_hdrs[i].name);
		printf("\t Size(in mem.): %d bytes (0x%08x)\n", section_hdrs[i].size, section_hdrs[i].size);
		printf("\t RVA: 0x%08x\n", section_hdrs[i].RVA);
		printf("\t Size(on disk): %d bytes (0x%08x)\n", section_hdrs[i].size_raw, section_hdrs[i].size_raw);
		printf("\t Start offset(on disk): 0x%08x\n", section_hdrs[i].ptr_data);
		printf("\t Relocations offset(on disk): 0x%08x\n", section_hdrs[i].ptr_relocs);
		printf("\t Line numbers offset(on disk): 0x%08x\n", section_hdrs[i].ptr_line);
		printf("\t Num. of relocations: %d\n", section_hdrs[i].num_relocs);
		printf("\t Num. of lines: %d\n", section_hdrs[i].num_line);
		printf("\t Characteristics: 0x%08x\n", section_hdrs[i].characteristics);
	}
}

int find_idata_section(PE_STANDARD_HEADER * std_hdr, PE_OPTIONAL_HEADER * opt_hdr, SECTION_HEADER * section_hdrs) {
	int i;

	/*
		Check each section header if it contains import table RVA, if it does it contains import data
	*/

	for (i = 0; i < std_hdr->num_of_sections; i++) {
		
		if ((opt_hdr->data_dirs[1].RVA < (section_hdrs[i].RVA + section_hdrs[i].size)) && (opt_hdr->data_dirs[1].RVA > section_hdrs[i].RVA))
			return i;

	}

	return -1;
}

int find_edata_section(PE_STANDARD_HEADER * std_hdr, PE_OPTIONAL_HEADER * opt_hdr, SECTION_HEADER * section_hdrs) {
	int i;

	/*
	Check each section header if it contains import table RVA, if it does it contains import data
	*/

	for (i = 0; i < std_hdr->num_of_sections; i++) {

		if ((opt_hdr->data_dirs[0].RVA < (section_hdrs[i].RVA + section_hdrs[i].size)) && (opt_hdr->data_dirs[0].RVA >= section_hdrs[i].RVA))
			return i;

	}
	return -1;
}