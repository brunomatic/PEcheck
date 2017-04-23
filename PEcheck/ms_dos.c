#include <stdio.h>
#include "ms_dos.h"


int read_ms_dos_header(FILE *file, MS_DOS_HEADER * header) {

	if (fseek(file, 0, SEEK_SET)) {
		return -1;
	}

	if (!fread(header, sizeof(MS_DOS_HEADER), 1, file)) {
		return -1;
	}

	return 1;

}

int print_ms_dos_header(MS_DOS_HEADER * header) {
	
	printf("====== MZ Header info ======\n");
	printf("\t Magic number: 0x%04x \n", header->magic_number);
	printf("\t Number of bytes on last page: %d(0x%04x) \n", header->bytes_last_page, header->bytes_last_page);
	printf("\t Number of pages: %d(0x%04x) \n", header->pages, header->pages);
	printf("\t Number of relocations entries: %d\n", header->num_relocs);
	printf("\t Header size(parag.): %d \n", header->header_size);
	printf("\t Extra parag. needed: %d - %d \n", header->min_extra, header->max_extra);
	printf("\t SS register init value: 0x%04x\n", header->ss_val);
	printf("\t SP register init value: 0x%04x\n", header->sp_val);
	printf("\t Checksum of executable: 0x%04x\n", header->checksum);
	printf("\t Entry point: 0x%04x%04x\n", header->cs_val, header->ip_val);
	printf("\t Relocation table offset: 0x%x\n", header->offset_relocs);
	printf("\t Overlay: 0x%04x\n", header->overlay);
	printf("\t OEM ID: 0x%04x\n", header->oem_id);
	printf("\t OEM info: 0x%04x\n", header->oem_info);
	printf("\t PE header offset: 0x%08x\n", header->pe_header_offset);
	
	return 1;
}