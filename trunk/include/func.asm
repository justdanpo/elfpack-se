        RTMODEL "ARMv4M", "USED"
        RTMODEL "ARMv4T", "USED"
        RTMODEL "ARMv5T", "USED"
        RTMODEL "StackAlign4", "USED"
        RTMODEL "__cpu_mode", "__pcs__interwork"
        RTMODEL "__data_model", "absolute"
        RTMODEL "__dlib_file_descriptor", "1"
        RTMODEL "__endian", "little"
        RTMODEL "__rt_version", "6"


	PUBLIC	ELF_BEGIN
	RSEG	ELFBEGIN:DATA
ELF_BEGIN

	RSEG	CODE:CODE
	PUBLIC	kill_data
	CODE32
kill_data
	BX	R1
        PUBLIC  seqkill
seqkill
        BX      R3

	PUBLIC  ASYNC
        PUBLIC  SYNC
_SYNC   DC32	0
_ASYNC	DC32	1
	
SYNC    DC32	_SYNC
ASYNC	DC32	_ASYNC
	

;=====================================================================================================
;
	#include "..\\include\DYN_tag.h"

DYNCONST        MACRO name,tag
        RSEG    DYN_CONST:DATA:NOROOT(2)
        PUBLIC  name
name:
        DC32     tag
        ENDM
;
        #include "..\\include\DYN_mac.asm"

        RSEG    DYN_CONST_DESC:DATA:ROOT(2)
        DC32    (SFE(DYN_CONST)-SFB(DYN_CONST))>>2

        RSEG    DYN_PAGE:DATA:NOROOT(2)

        RSEG    DYN_PAGE_DESC:DATA:ROOT(2)
        DC32    (SFE(DYN_PAGE)-SFB(DYN_PAGE))>>3
	END
