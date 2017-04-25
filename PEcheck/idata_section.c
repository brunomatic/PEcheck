#include <stdio.h>
#include <stdlib.h>
#include "idata_section.h"



/*
	Builds directory list structure from file - it is a mes but works
	I'll fix it later...maybe
*/
int read_directory_table(FILE * file,  DIRECTORY_TABLE * table, uint32_t dir_tbl_RVA, uint32_t section_RVA, uint32_t section_file_offest, int32_t num_entries) {
	int32_t i, j, k;
	uint8_t c;
	uint32_t offset, temp;

	// directory table offset = RVA(import table) - RVA(section) + ptr_to_raw_data(section)
	offset = dir_tbl_RVA - section_RVA + section_file_offest;
	if (fseek(file, offset, SEEK_SET)) {
		return 0;
	}

	// allocate data and read raw directory table
	table->data = malloc(sizeof(DIRECTORY_ENTRY)*num_entries);
	table->num_entries = num_entries;

	for (i = 0; i < num_entries; i++)
	{
		if (!fread(&(table->data[i]), DIR_ENTRY_SIZE, 1, file)) {
			return 0;
		}

	}


	// read imports name
	for ( i = 0; i < num_entries; i++)
	{
		// name offset = RVA(name) - RVA(section) +  ptr_to_raw_data(section)
		offset = table->data[i].name_RVA - section_RVA + section_file_offest;
		if (fseek(file, offset, SEEK_SET)) {
			return 0;
		}

		// read the name sign by sign - god why
		j = 0;
		c = 1;
		while (c!=0)
		{
			if (!fread(&c, sizeof(uint8_t), 1, file)) {
				return 0;
			}
			table->data[i].name[j] = c;
			j++;
		}
	}
	

	// find out how many entries are ther in import table for each directory entry
	// then allocate space for them and bind them to that directory entry
	// they just couldn't use 2 extra bytes to write this somewhere
	for ( i = 0; i < num_entries; i++)
	{
		offset = table->data[i].import_lookup_table_RVA - section_RVA + section_file_offest;
		if (fseek(file, offset, SEEK_SET)) {
			return 0;
		}

		table->data[i].num_import_tbl_entries = 0;
		temp = 1;
		
		while (temp != 0)
		{
			if (!fread(&temp, sizeof(uint32_t), 1, file)) {
				return 0;
			}
			table->data[i].num_import_tbl_entries ++;
		}
		// don't count the null entry
		table->data[i].num_import_tbl_entries--;
	}


	// read the bitfield from import table for each entry
	for (i = 0; i < num_entries; i++)
	{
		offset = table->data[i].import_lookup_table_RVA - section_RVA + section_file_offest;
		if (fseek(file, offset, SEEK_SET)) {
			return 0;
		}

		table->data[i].import_tbl = malloc(table->data[i].num_import_tbl_entries * sizeof(IMPORT_LOOKUP_ENTRY));

		for (j = 0; (uint32_t)j < table->data[i].num_import_tbl_entries; j++)
		{
			if (!fread(&(table->data[i].import_tbl[j]), IMPORT_TABLE_ENTRY_SIZE, 1, file)) {
				return 0;
			}
		}
		
	}

	// read ordinal or hint + name
	// if 31 - 1 - ordinal --> 15-0 matter
	// if 31 - 0 - name RVA--> 30-0 matter 
	for (i = 0; i < num_entries; i++)
	{
		for (j = 0; (uint32_t)j < table->data[i].num_import_tbl_entries; j++)
		{
			// check if ordinal, if so set name to null
			if (table->data[i].import_tbl[j].bitfield & 0x80000000) {
				table->data[i].import_tbl[j].name[0] = '\0';
			}
			else
			{
				offset = table->data[i].import_tbl[j].bitfield - section_RVA + section_file_offest;
				if (fseek(file, offset, SEEK_SET)) {
					return 0;
				}
				// read the index
				if (!fread(&(table->data[i].import_tbl[j].hint), sizeof(uint16_t), 1, file)) {
					return 0;
				}

				// read the name sign by sign - god why
				k = 0;
				c = 1;
				while ((c != 0) && (k!=255))
				{
					if (!fread(&c, sizeof(uint8_t), 1, file)) {
						return 0;
					}
					table->data[i].import_tbl[j].name[k] = c;
					k++;
				}
			}
		}
	}	

	return 1;

}

void print_directory_table(DIRECTORY_TABLE * table, int32_t offset) {
	int i, j;

	printf("====== Directory table ======\n");

	for ( i = 0; i < table->num_entries; i++)
	{
		printf("==========================================\n");
		printf("\t Import name: %s \n", table->data[i].name);
		printf("\t Time-data stamp: 0x%08x\n", table->data[i].time_date_stamp);
		printf("\t Import Lookup Table: 0x%08x (phys:0x%08x)\n", table->data[i].import_adr_table_RVA, table->data[i].import_adr_table_RVA + offset);
		printf("\t Import Address Table: 0x%08x (phys:0x%08x)\n", table->data[i].import_adr_table_RVA, table->data[i].import_adr_table_RVA + offset);
		printf("\t Forwarder Chain: 0x%08x\n", table->data[i].forwarder_chain);
		printf("\t\t ====== Import Lookup Table ======\n");
		printf("\t\t N \t Bitfield \t\t\t Hint \t\t Name\n");
		for ( j = 0; (uint32_t)j < table->data[i].num_import_tbl_entries; j++)
		{
			printf("\t\t %d \t 0x%08x (phys:0x%08x) \t 0x%04x \t %s \n", j+1, table->data[i].import_tbl[j].bitfield, \
				(table->data[i].import_tbl[j].bitfield & 0x80000000 ? 0x00000000 : table->data[i].import_tbl[j].bitfield + offset), \
				table->data[i].import_tbl[j].hint, table->data[i].import_tbl[j].name);
		}

	}


}