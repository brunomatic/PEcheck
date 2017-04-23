#include <stdio.h>
#include <time.h>
#include "pe_std.h"

void machine_type_lookup(uint16_t machine);
void timestamp(uint32_t timestamp);
void characteristics(uint16_t characteristics);

int read_pe_header(FILE *file, PE_STANDARD_HEADER * header, int32_t offset) {

	uint32_t signature;

	if (fseek(file, offset, SEEK_SET)) {
		return -1;
	}

	if (!fread(&signature, sizeof(uint32_t), 1, file)) {
		return -1;
	}

	if (signature != 0x4550) {
		return -1;
	}

	if (!fread(header, sizeof(PE_STANDARD_HEADER), 1, file)) {
		return -1;
	}

	return 1;
}

int print_pe_std_header(PE_STANDARD_HEADER * header) {

	printf("====== PE Header info ======\n");
	printf("\t Machine type: ");
	machine_type_lookup(header->machine);
	printf("(0x%04x)\n", header->machine);
	printf("\t Number of sections: %d (0x%04x)\n", header->num_of_sections, header->num_of_sections);

	printf("\t Time-date stamp: ");
	timestamp(header->time_date_stamp);
	printf("\t Symbol table offset: 0x%08x\n", header->ptr_to_symbol_tbl);
	printf("\t Number of symbols: %d(0x%08x)\n", header->num_of_symbols, header->num_of_symbols);
	printf("\t Size of optional header: 0x%04x\n", header->size_of_opt_header);
	printf("\t Characteristics: 0x%04x\n", header->characteristics);
	characteristics(header->characteristics);

	return 1;
}


/* Function is a lookup table for machine type */
void machine_type_lookup(uint16_t machine) {
	switch (machine) {
	case 0x1d3:
		printf("Matsushita AM33");
		break;
	case 0x8664:
		printf("x64");
		break;
	case 0x1c0:
		printf("ARM little endian");
		break;
	case 0xaa64:
		printf("ARM64 little endian");
		break;
	case 0x1c4:
		printf("ARM Thumb-2 little endian");
		break;
	case 0xebc:
		printf("EFI byte code");
		break;
	case 0x14c:
		printf("Intel 386 or later processors and compatible processors");
		break;
	case 0x200:
		printf("Intel Itanium processor family");
		break;
	case 0x9041:
		printf("Mitsubishi M32R little endian");
		break;
	case 0x266:
		printf("MIPS16");
		break;
	case 0x366:
		printf("MIPS with ");
		break;
	case 0x466:
		printf("MIPS16 with FPU");
		break;
	case 0x1f0:
		printf("Power PC little endian");
		break;
	case 0x1f1:
		printf("Power PC with floating point support");
		break;
	case 0x166:
		printf("MIPS little endian");
		break;
	case 0x5032:
		printf("RISC-V 32-bit address space");
		break;
	case 0x5064:
		printf("RISC-V 64-bit address space");
		break;
	case 0x5128:
		printf("RISC-V 128-bit address space");
		break;
	case 0x1a2:
		printf("Hitachi SH3");
		break;
	case 0x1a3:
		printf("Hitachi SH3 DSP");
		break;
	case 0x1a6:
		printf("Hitachi SH4");
		break;
	case 0x1a8:
		printf("Hitachi SH5");
		break;
	case 0x1c2:
		printf("Thumb");
		break;
	case 0x169:
		printf("MIPS little-endian WCE v2");
		break;
	default:
		printf("Any/unknown machine type");
	}
	return;
}

/* Prints time in readable format from timestamp*/
void timestamp(uint32_t timestamp)
{
	char timebuf[26];
	errno_t err;
	time_t ltime; /* calendar time */
	ltime = (time(NULL)&0xFFFFFFFF00000000);
	ltime = ltime | timestamp;
	err = ctime_s(timebuf, 26, &ltime);
	printf("0x%08x - %s", timestamp, timebuf);
}

/* Prints all found characteristics */
void characteristics(uint16_t characteristics) {
	if (0x0001 & characteristics) printf("\t\tRELOCS_STRIPPED\n");
	if (0x0002 & characteristics) printf("\t\tEXECUTABLE_IMAGE\n");
	if (0x0004 & characteristics) printf("\t\tLINE_NUMS_STRIPPED\n");
	if (0x0008 & characteristics) printf("\t\tLOCAL_SYMS_STRIPPED\n");
	if (0x0010 & characteristics) printf("\t\tAGGRESSIVE_WS_TRIM\n");
	if (0x0020 & characteristics) printf("\t\tLARGE_ADDRESS_ AWARE\n");
	if (0x0080 & characteristics) printf("\t\tBYTES_REVERSED_LO\n");
	if (0x0100 & characteristics) printf("\t\t32BIT_MACHINE\n");
	if (0x0200 & characteristics) printf("\t\tDEBUG_STRIPPED\n");
	if (0x0400 & characteristics) printf("\t\tREMOVABLE_RUN_ FROM_SWAP\n");
	if (0x0800 & characteristics) printf("\t\tNET_RUN_FROM_SWAP\n");
	if (0x1000 & characteristics) printf("\t\tSYSTEM\n");
	if (0x2000 & characteristics) printf("\t\tDLL\n");
	if (0x4000 & characteristics) printf("\t\tUP_SYSTEM_ONLY\n");
	if (0x8000 & characteristics) printf("\t\tBYTES_REVERSED_HI\n");
}