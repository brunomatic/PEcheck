#ifndef PE_STD_HEADER
#define PE_STD_HEADER

#include <stdint.h>

typedef struct {
	uint16_t machine;				// defines target machine
	uint16_t num_of_sections;		// number of sections
	uint32_t time_date_stamp;		// time-data stamp(lower 32 bits)
	uint32_t ptr_to_symbol_tbl;		// offset to symblos table
	uint32_t num_of_symbols;		// number of entries in symbol table
	uint16_t size_of_opt_header;	// size of optional header
	uint16_t characteristics;		// flags 
} PE_STANDARD_HEADER;

int read_pe_header(FILE *file, PE_STANDARD_HEADER * header, int32_t offset);

int print_pe_std_header(PE_STANDARD_HEADER * header);

#endif