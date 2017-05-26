#ifndef  EDATA_SECTION_HEADER
#define EDATA_SECTION_HEADER

#include <stdint.h>

typedef struct {

	uint32_t flags;				// reserved - 0
	uint32_t time_date_stamp;	// obvious
	uint16_t major_v;			// major version number
	uint16_t minor_v;			// minor version number
	uint32_t name_RVA;			// adress of name string - relative to image base
	uint32_t ordinal_base;		// starting ordinal number
	uint32_t adr_table_entries; // number of entries in export address table
	uint32_t num_name_ptr;		// number of name pointers(and ordinal table entries)
	uint32_t address_tbl_RVA;	// address of export address table - relative to image base
	uint32_t name_ptr_RVA;		// address of name pointers field - relative to image base
	uint32_t ordinal_tbl_RVA;	// address of ordinal table - relative to image base
} EXPORT_DIRECTORY_TABLE;

typedef struct {
	uint32_t RVA;				// export or forwarder RVA - usually start of the function
} EXPORT_ADDRESS_TABLE_ENTRY;

typedef struct {
	uint32_t RVA;				// bunch of pointers to name table entries(names of functions)
} EXPORT_NAME_POINTER_TABLE_ENTRY;

typedef struct {
	uint16_t ordinal;			// ordinal + ordinal_base(see export directory table)
} EXPORT_ORDINAL_TABLE_ENTRY;

typedef struct {
	uint8_t name[255];			// dont care - if you have a function name longer than 255 char, think about your life 
}EXPORT_NAME_TABLE_ENTRY;

typedef struct {
	uint8_t name[255];
	EXPORT_DIRECTORY_TABLE dir_table;
	EXPORT_ADDRESS_TABLE_ENTRY * adr_tbl;
	EXPORT_NAME_POINTER_TABLE_ENTRY * name_ptr_tbl;
	EXPORT_ORDINAL_TABLE_ENTRY * ordinal_tbl;
	EXPORT_NAME_TABLE_ENTRY * name_tbl;
} EDATA_SECTION;

int read_edata_directory_table(FILE * file, EDATA_SECTION ** section_ptr, uint32_t tbl_RVA, uint32_t section_RVA, uint32_t section_file_offest);

void print_edata_directory_table(EDATA_SECTION * section, int32_t offset);

#endif