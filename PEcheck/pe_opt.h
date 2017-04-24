#ifndef PE_OPT_HEADER
#define PE_OPT_HEADER

#include <stdint.h>
#include <stdio.h>

typedef struct
{
	uint16_t magic;				// magic number
	uint8_t major_linker_v;		// major linker version
	uint8_t minor_linker_v;		// minor linker version
	uint32_t size_code;			// size of .text section
	uint32_t size_init_data;	// size of .data section
	uint32_t size_uninit_data;	// size of .bss section
	uint32_t adr_entry_point;	// address of entry point - relative to image base
	uint32_t base_code;			// address of star of code section - relative to image base
	uint32_t base_data;			// address of star of data section - relative to image base
	uint32_t image_base;		// IMAGE BASE address - first byte of image in memory is here
	uint32_t section_align;		// alignment of sections in bytes - when loaded
	uint32_t file_align;		// alignment of sections in bytes - on disk/ in file
	uint16_t major_OS_v;		// major OS version
	uint16_t minor_OS_v;		// minor OS version
	uint16_t major_image_v;		// major image version
	uint16_t minor_image_v;		// minor image version
	uint16_t major_subsystem_v;	// major subsystem version
	uint16_t minor_subsystem_v;	// minor subsystem version
	uint32_t win32_v;			// reserved - 0x0
	uint32_t size_image;		// image size - when loaded
	uint32_t size_headers;		// size of all headers
	uint32_t checksum;			// image checksum
	uint16_t subsystem;			// required subystem for running
	uint16_t dll_character;		// DLL characteristics
	uint32_t size_stack_resv;	// size to reserve on stack
	uint32_t size_stack_commit;	// size to commit on stack
	uint32_t size_heap_resv;	// size to reserve on heap
	uint32_t size_heap_commit;	// size to commit on heap
	uint32_t loader_flags;		// reserved - 0x0
	uint32_t num_data_dirs;		// number of data directories after optional header
} PE_OPTIONAL_HEADER;

int read_optional_header(FILE * file, PE_OPTIONAL_HEADER * header, uint32_t offset);

void print_pe_opt_header(PE_OPTIONAL_HEADER * header);

#endif // !PE_OPT_HEADER
