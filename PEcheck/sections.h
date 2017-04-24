#ifndef SECTIONS_HEADER_H
#define SECTIONS_HEADER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
	uint8_t name[8];	// name of section
	uint32_t size;		// size of section - when loaded in memory
	uint32_t VA;		// virtual adress of start of a section - relative to image base
	uint32_t size_raw;	// raw section size - on disk
	uint32_t ptr_data;	// pointer to start of section in raw offset
	uint32_t ptr_relocs;// pointer to relocation table
	uint32_t ptr_line;	// pointer to line numbers ?
	uint16_t num_relocs;// number or relocations in table
	uint16_t num_line;	// number of line numbers ?
	uint32_t characteristics; // characteristics flags
} SECTION_HEADER;

typedef struct {
	uint32_t import_lookup_table_RVA;	// names or ordinal of each import function
	uint32_t time_date_stamp;			// timestamp - don't care
	uint32_t forwarder_chain;			// function forwarding sorcery - don't care
	uint32_t name_RVA;					// pointer to DLL name - relative to image base
	uint32_t import_adr_table_RVA;		// names or ordinal of each import function - first thunk
} DIRECTORY_ENTRY;

typedef struct {
	uint32_t bitfield;					// 31 - if set ordinal, else name
} IMPORT_LOOKUP_ENTRY;					// 15-0 - ordinal or 30-0 rva of hint table entry

typedef struct {
	uint8_t hint;
	uint8_t * name;
} HINT_ENTRY;

typedef struct {
	DIRECTORY_ENTRY * directory_table;
	IMPORT_LOOKUP_ENTRY * import_lookup_tables;
	HINT_ENTRY * hint_table;
} idata_section;

int read_section_table(FILE *file, SECTION_HEADER ** headers, uint8_t num_sections, uint32_t offset);

int find_import_section(FILE *file, PE_STANDARD_HEADER * std_hdr, PE_OPTIONAL_HEADER * opt_hdr, SECTION_HEADER * section_hdrs);

#endif // ! SECTIONS_HEADER
