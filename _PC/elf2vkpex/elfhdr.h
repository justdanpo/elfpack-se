#include "stdint.h"

struct ELFHDR
{
	uint8_t Signature[4];
	uint8_t File_class;
	uint8_t Data_encoding;
	uint8_t File_version;
	uint8_t unk[9];
	uint16_t Type;
	uint16_t Machine;
	uint32_t Module_version;
	uint32_t Entry_point;
	uint32_t Program_header_table;
	uint32_t Section_header_table;
	uint32_t Module_flags;
	uint16_t Size_of_header;
	uint16_t Program_header_table_entry_size;
	uint16_t Program_header_table_entry_count;
	uint16_t Section_header_table_entry_size;
	uint16_t Section_header_table_entry_count;
	uint16_t String_table_index;
};

enum PTENTRYTYPE
{
	PTET_NULL,
	PTET_LOAD,
	PTET_DYN,
	PTET_INTP,
	PTET_NOTE,
	PTET_SHLB,
	PTET_PHDR
};

struct PROGTBLENTRY
{
	uint32_t Type;
	uint32_t Physical_offset;
	uint32_t Virtual_address;
	uint32_t Physical_address;
	uint32_t Size_in_file;
	uint32_t Size_in_memory;
	uint32_t Attributes;
	uint32_t Section_alignment;
};
