#include <stdio.h>
#include <stdlib.h>
#include "edata_section.h"



/*
	Double pointers, oh joy ;) - it is a mess...again, but hey it is for free and nobody is ever gonna use it
*/
int read_edata_directory_table(FILE * file, EDATA_SECTION ** section_ptr, uint32_t tbl_RVA, uint32_t section_RVA, uint32_t section_file_offest) {
	int32_t i, j;
	uint8_t c;
	uint32_t offset;
	EDATA_SECTION * section;

	section = (EDATA_SECTION *)malloc(sizeof(EDATA_SECTION));

	// directory table offset = RVA(import table) - RVA(section) + ptr_to_raw_data(section)
	offset = tbl_RVA - section_RVA + section_file_offest;
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	if (!fread(&(section->dir_table), sizeof(EXPORT_DIRECTORY_TABLE), 1, file)) {
		return 0;
	}


	// allocate space for all the tables
	section->adr_tbl = (EXPORT_ADDRESS_TABLE_ENTRY*)malloc(sizeof(EXPORT_ADDRESS_TABLE_ENTRY)*section->dir_table.adr_table_entries);
	section->name_ptr_tbl = (EXPORT_NAME_POINTER_TABLE_ENTRY *)malloc(sizeof(EXPORT_NAME_POINTER_TABLE_ENTRY)*section->dir_table.num_name_ptr);
	section->ordinal_tbl = (EXPORT_ORDINAL_TABLE_ENTRY *)malloc(sizeof(EXPORT_ORDINAL_TABLE_ENTRY)*section->dir_table.num_name_ptr);
	section->name_tbl = (EXPORT_NAME_TABLE_ENTRY *)malloc(sizeof(EXPORT_NAME_TABLE_ENTRY)*section->dir_table.num_name_ptr);



	// read addresse table 
	offset = section->dir_table.address_tbl_RVA - section_RVA + section_file_offest;
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}
	
	if (!fread(section->adr_tbl, sizeof(EXPORT_ADDRESS_TABLE_ENTRY), section->dir_table.adr_table_entries, file)) {
		return 0;
	}


	// read name pointer table
	offset = section->dir_table.name_ptr_RVA - section_RVA + section_file_offest;
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	if (!fread(section->name_ptr_tbl, sizeof(EXPORT_NAME_POINTER_TABLE_ENTRY), section->dir_table.num_name_ptr, file)) {
		return 0;
	}

	// read ordinal table
	offset = section->dir_table.ordinal_tbl_RVA - section_RVA + section_file_offest;
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	if (!fread(section->ordinal_tbl, sizeof(EXPORT_ORDINAL_TABLE_ENTRY), section->dir_table.num_name_ptr, file)) {
		return 0;
	}

	// handle names from name ptr table
	for ( i = 0; i < (int32_t)section->dir_table.num_name_ptr; i++)
	{
		// skip if not defined
		if (section->name_ptr_tbl[i].RVA == 0) 
			continue;

		offset = section->name_ptr_tbl[i].RVA - section_RVA + section_file_offest;
		if (fseek(file, offset, SEEK_SET)) {
			return 0;
		}

		// read the name sign by sign - god why
		j = 0;
		c = 1;
		while (c != 0)
		{
			if (!fread(&c, sizeof(uint8_t), 1, file)) {
				return 0;
			}
			section->name_tbl[i].name[j] = c;
			j++;
		}

	}
	
	// pickup the name also
	offset = section->dir_table.name_RVA - section_RVA + section_file_offest;
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	// read the name sign by sign - god why
	j = 0;
	c = 1;
	while (c != 0)
	{
		if (!fread(&c, sizeof(uint8_t), 1, file)) {
			return 0;
		}
		section->name[j] = c;
		j++;
	}


	// connect those pointers

	*section_ptr = section;

	return 1;

}

void print_edata_directory_table(EDATA_SECTION * section, int32_t offset) {

	int32_t i;

	printf("====== edata section - EXPORTS ======\n");

	printf("\t Flags: 0x%08x \n", section->dir_table.flags);
	printf("\t Timestamp: 0x%08x \n", section->dir_table.time_date_stamp);
	printf("\t Version: 0x%04x,0x%04x \n", section->dir_table.major_v, section->dir_table.minor_v);
	printf("\t Name: %s \n", section->name);
	printf("\t Ordinal base: 0x%08x \n", section->dir_table.ordinal_base);
	printf("\t Address table entries: %d \n", section->dir_table.adr_table_entries);
	printf("\t Name pointer entries: %d \n", section->dir_table.num_name_ptr);
	printf("\t Address table: 0x%08x (RVA: 0x%08x)\n", section->dir_table.address_tbl_RVA + offset, section->dir_table.address_tbl_RVA);
	printf("\t Name pointer table: 0x%08x (RVA: 0x%08x)\n", section->dir_table.name_ptr_RVA  + offset, section->dir_table.name_ptr_RVA); 
	printf("\t Ordinal table: 0x%08x (RVA: 0x%08x)\n", section->dir_table.ordinal_tbl_RVA + offset, section->dir_table.ordinal_tbl_RVA);

	printf("\t\t %-10s \t %-10s \t %-25s \t %-10s \n", "Ordinal", "Name RVA", "Name", "Export RVA");
	for (i = 0; i < section->dir_table.num_name_ptr; i++)
	{
		// according to MS docs it should be (section->ordinal_tbl[i].ordinal - section->dir_table.ordinal_base) but it is wrong
		printf("\t\t %-10d \t (0x%08x) \t %-25s \t  0x%08x \n", section->ordinal_tbl[i].ordinal, section->name_ptr_tbl[i].RVA, section->name_tbl[i].name, section->adr_tbl[section->ordinal_tbl[i].ordinal].RVA);
	}

	return;
}