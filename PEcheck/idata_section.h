#ifndef  IDATA_SECTION_HEADER
#define IDATA_SECTION_HEADER

#include <stdint.h>
#include "sections.h"

/* 
	Raw data structures - for easy read-in
*/

#define DIR_ENTRY_SIZE			(sizeof(uint32_t)*5)
#define IMPORT_TABLE_ENTRY_SIZE	sizeof(uint32_t)

typedef struct {
	uint32_t bitfield;					// 31 - if set ordinal, else name, 15-0 - ordinal or 30-0 rva of hint table entry
	uint16_t hint;						// avoiding to create a HINT typedef
	char  name[255];					// fixed size of name - i don't care ;)
} IMPORT_LOOKUP_ENTRY;

typedef struct {
	uint32_t import_lookup_table_RVA;	// names or ordinal of each import function
	uint32_t time_date_stamp;			// timestamp - don't care
	uint32_t forwarder_chain;			// function forwarding sorcery - don't care
	uint32_t name_RVA;					// pointer to DLL name - relative to image base
	uint32_t import_adr_table_RVA;		// names or ordinal of each import function - first thunk
	IMPORT_LOOKUP_ENTRY * import_tbl;
	uint32_t num_import_tbl_entries;
	char name[255];						// again i don't care
} DIRECTORY_ENTRY;			


typedef struct {
	DIRECTORY_ENTRY * data;
	uint16_t num_entries;
} DIRECTORY_TABLE;


int read_directory_table(FILE * file, DIRECTORY_TABLE * table, uint32_t dir_tbl_RVA, uint32_t section_RVA, uint32_t section_file_offest, int32_t num_entries);

void print_directory_table(DIRECTORY_TABLE * table);

#endif // ! IDATA_SECTION_HEADER
