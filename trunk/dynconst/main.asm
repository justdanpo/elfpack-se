#define UNDEFINED 0xFFFFFFFF
#include "temp\temp.h"
DYNCONST MACRO   val,tag
        org tag*4
        DCD val
        ENDM

#include "..\include\DYN_tag.h"
#include "..\include\DYN_mac.asm"

  END
