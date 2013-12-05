//#include "..\include\DevDef.h"
        RTMODEL "ARMv4M", "USED"
        RTMODEL "ARMv4T", "USED"
        RTMODEL "ARMv5T", "USED"
        RTMODEL "StackAlign4", "USED"
        RTMODEL "__cpu_mode", "__pcs__interwork"
        RTMODEL "__data_model", "absolute"
        RTMODEL "__dlib_file_descriptor", "0"
        RTMODEL "__endian", "little"
        RTMODEL "__rt_version", "6"

        EXTERN  Library

        RSEG CODE:CODE:NOROOT(2)



call    MACRO   a,b
        PUBLIC  a
a:
        CODE32
        LDR     R12,=Library-0x400
        LDR     R12,[R12,#b*4]
        BX      R12
        ENDM

defadr  MACRO   a,b
        PUBLIC  a
a       EQU     b
        ENDM

        call    List_IndexOf,0x17B
        call    AllocDirHandle,0x11D
        call    GetFname,0x11E
        call    memset,0x112
        call    memcpy,0x113
        call    List_Create,0x169
        call    List_Find,0x1AC
        call    List_Get,0x1AD
        call    List_InsertFirst,0x16B
        call    List_RemoveAt,0x179
        call    List_InsertLast,0x1B0
        call    wstrrchr,0x1AE
        call    _fopen,0x116
        call    wstrlen,0x157
        call    wstrcpy,0x152
        call    wstrcat,0x154
        call    debug_printf,0x1B1
        call    DestroyDirHandle,0x254
        call    set_envp,0x18E
        call    get_bid,0x18F
        call    current_process,0x184

   END
