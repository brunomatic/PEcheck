#ifndef MS_DOS_HEADER
#define MS_DOS_FUNC_HEADER

#include <stdint.h>

typedef struct {
	uint16_t magic_number;			// magic number
	uint16_t bytes_last_page;		// number of bytes on last page( maybe less than 512)
	uint16_t pages;					// number of pages - file size%512
	uint16_t num_relocs;			// number of possible relocations
	uint16_t header_size;			// header size in paragraphs(16 bytes)
	uint16_t min_extra;				// minimal extra needed paragraphs
	uint16_t max_extra;				// maximal extra needed paragraphs
	uint16_t ss_val;				// SS register initial value
	uint16_t sp_val;				// SP register initial value
	uint16_t checksum;				// checksum of exe - can be 0
	uint16_t ip_val;				// IP register initial value
	uint16_t cs_val;				// CS register initial value - entry = CS:IP ?
	uint16_t offset_relocs;			// offset of relocations table
	uint16_t overlay;				// overlay number ?
	uint16_t resv_wrd0[4];			// reserved - 4
	uint16_t oem_id;				// OEM ID
	uint16_t oem_info;				// OEM INFO
	uint16_t resv_wrd1[10];			// reserved - 10
	int32_t  pe_header_offset;		// PE header offset
} MS_DOS_HEADER;

int read_ms_dos_header(FILE *file, MS_DOS_HEADER * header);

int print_ms_dos_header(MS_DOS_HEADER * header);

#endif // MS_DOS_HEADER
