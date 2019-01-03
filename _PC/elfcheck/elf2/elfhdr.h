struct ELF
{
	DWORD Signature;
	BYTE File_class;
	BYTE Data_encoding;
	BYTE File_version;
	BYTE unk[9];
	WORD Type;
	WORD Machine;
	DWORD Module_version;
	DWORD Entry_point;
	DWORD Program_header_table;
	DWORD Section_header_table;
	DWORD Module_flags;
	WORD Size_of_header;
	WORD Program_header_table_entry_size;
	WORD Program_header_table_entry_count;
	WORD Section_header_table_entry_size;
	WORD Section_header_table_entry_count;
	WORD String_table_index;
};

struct PROGTBLENTRY
{
	DWORD Type;
	DWORD Physical_offset;
	DWORD Virtual_address;
	DWORD Physical_address;
	DWORD Size_in_file;
	DWORD Size_in_memory;
	DWORD Attributes;
	DWORD Section_alignment;
};
