#include <stdio.h>
#include <stdlib.h>
#include "pe_opt.h"


void subystem_lookup(uint16_t subsystem);
void dll_characteristics(uint16_t characteristics);

int read_optional_header(FILE * file, PE_OPTIONAL_HEADER * header, uint32_t offset) {

	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	if (!fread(header, sizeof(PE_OPTIONAL_HEADER)-4, 1, file)) {
		return 0;
	}

	if (header->magic != 0x10b) {
		return 0;
	}

	header->data_dirs = malloc(sizeof(DATA_DIR)*header->num_data_dirs);
	
	if (!fread(header->data_dirs, sizeof(DATA_DIR), header->num_data_dirs, file)) {
		return 0;
	}

	return 1;
}


void print_pe_opt_header(PE_OPTIONAL_HEADER * header) {
	const char * names[] = {
		"Export Table",
		"Import Table",
		"Resource Table",
		"Exception Table",
		"Certificate Table",
		"Relocation Table",
		"Debug",
		"Architecture",
		"Global Ptr",
		"TLS Table",
		"Load Config Table",
		"Bound Import",
		"IAT",
		"Delay Import Descriptor",
		"CLR Runtime Header",
		"Reserved"
	};
	uint8_t i;

	printf("====== Optional Header info ======\n");
	printf("\t Magic number: 0x%04x\n", header->magic);

	printf("\t Linker version:\n"); 
	printf("\t\t Major: 0x%02x\n", header->major_linker_v);
	printf("\t\t Minor: 0x%02x\n", header->minor_linker_v);

	printf("\t Size of .text section: %d bytes (0x%08x)\n", header->size_code, header->size_code);
	printf("\t Size of .data section: %d bytes (0x%08x)\n", header->size_init_data, header->size_init_data);
	printf("\t Size of .bss section: %d bytes (0x%08x)\n", header->size_uninit_data, header->size_uninit_data);

	printf("\t Entry point: 0x%08x (phys: 0x%08x)\n", header->adr_entry_point, header->adr_entry_point + header->image_base );

	printf("\t Start of .text: 0x%08x (phys: 0x%08x)\n", header->base_code, header->base_code + header->image_base);
	printf("\t Start of .data: 0x%08x (phys: 0x%08x)\n", header->base_data, header->base_data + header->image_base);

	printf("\t Image base: 0x%08x \n", header->image_base);

	printf("\t Section alignment:\n");
	printf("\t\t In memory: %d bytes (0x%08x)\n", header->section_align, header->section_align);
	printf("\t\t On disk: %d bytes (0x%08x)\n", header->file_align, header->file_align);

	printf("\t OS version:\n");
	printf("\t\t Major: 0x%04x\n", header->major_OS_v);
	printf("\t\t Minor: 0x%04x\n", header->minor_OS_v);

	printf("\t Image version:\n");
	printf("\t\t Major: 0x%04x\n", header->major_image_v);
	printf("\t\t Minor: 0x%04x\n", header->minor_image_v);

	printf("\t Subsystem version:\n");
	printf("\t\t Major: 0x%04x\n", header->major_subsystem_v);
	printf("\t\t Minor: 0x%04x\n", header->minor_subsystem_v);

	printf("\t Image size: %d bytes (0x%08x)\n", header->size_image, header->size_image);
	printf("\t Headers size: %d bytes (0x%08x)\n", header->size_headers, header->size_headers);

	printf("\t Image checksum: 0x%08x\n", header->checksum);

	printf("\t Subsystem: ");
	subystem_lookup(header->subsystem);
	printf(" (0x%04x)\n", header->subsystem);

	printf("\t DLL characteristics: 0x%04x\n", header->dll_character);
	dll_characteristics(header->dll_character);


	printf("\t Stack:\n");
	printf("\t\t Reserve: %d bytes (0x%08x)\n", header->size_stack_resv, header->size_stack_resv);
	printf("\t\t Commit: %d bytes (0x%08x)\n", header->size_stack_commit, header->size_stack_commit);

	printf("\t Heap:\n");
	printf("\t\t Reserve: %d bytes (0x%08x)\n", header->size_heap_resv, header->size_heap_resv);
	printf("\t\t Commit: %d bytes (0x%08x)\n", header->size_heap_commit, header->size_heap_commit);

	printf("\t Loader flags: 0x%08x\n", header->loader_flags);

	printf("\t Number of data directories: %d (0x%08x)\n", header->num_data_dirs, header->num_data_dirs);

	printf("\t========================= Data directories =========================\n");
	printf("\t%-20s", "Name"); 
	printf("\t%-8s", "RVA");
	printf("\t%-8s", "Phys");
	printf("\t%-8s", "Size(bytes)\n");
	printf("\t---------------------------------------------------------------------\n");

	for (i = 0; i < header->num_data_dirs; i++)
	{
		printf("\t%-20s\t0x%08x\t0x%08x\t%d(0x%08x)\n", names[i], header->data_dirs[i].RVA, header->data_dirs[i].RVA + header->image_base, header->data_dirs[i].size, header->data_dirs[i].size);
	}
}


/* Function is lookup table for subsytem value*/
void subystem_lookup(uint16_t subsystem) {
	switch (subsystem) {
	case 1:
		printf("NATIVE");
		break;
	case 2:
		printf("WINDOWS_GUI");
		break;
	case 3:
		printf("WINDOWS_CUI");
		break;
	case 5:
		printf("OS2_CUI");
		break;
	case 7:
		printf("POSIX_CUI");
		break;
	case 8:
		printf("NATIVE_WINDOWS");
		break;
	case 9:
		printf("WINDOWS_CE_GUI");
		break;
	case 10:
		printf("EFI_APPLICATION");
		break;
	case 11:
		printf("EFI_BOOT_ SERVICE_DRIVE");
		break;
	case 12:
		printf("EFI_RUNTIME_ DRIVER");
		break;
	case 13:
		printf("EFI_ROM");
		break;
	case 14:
		printf("XBOX");
		break;
	case 16:
		printf("WINDOWS_BOOT_APPLICATION");
		break;
	default:
		printf("UNKNOWN");
	}
}

/* Function prints found caaracteristics flags*/
void dll_characteristics(uint16_t characteristics) {
	if (characteristics & 0x0020) printf("\t\t HIGH_ENTROPY_VA\n");;
	if (characteristics & 0x0040) printf("\t\t DYNAMIC_BASE\n");;
	if (characteristics & 0x0080) printf("\t\t FORCE_INTEGRITY\n");;
	if (characteristics & 0x0100) printf("\t\t NX_COMPAT\n");;
	if (characteristics & 0x0200) printf("\t\t NO_ISOLATION\n");;
	if (characteristics & 0x0400) printf("\t\t NO_SEH\n");;
	if (characteristics & 0x0800) printf("\t\t NO_BIND\n");;
	if (characteristics & 0x1000) printf("\t\t APPCONTAINER\n");;
	if (characteristics & 0x2000) printf("\t\t  WDM_DRIVER\n");;
	if (characteristics & 0x4000) printf("\t\t GUARD_CF\n");;
	if (characteristics & 0x8000) printf("\t\t TERMINAL_SERVER_AWARE\n");;
}