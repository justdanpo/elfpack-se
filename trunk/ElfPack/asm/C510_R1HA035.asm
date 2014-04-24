//C510_R1HA035
#include "target.h"
        RSEG   CODE
        CODE32

defadr  MACRO   a,b
        PUBLIC  a
a       EQU     b
        ENDM

        RSEG  CODE
        defadr   STANDBY_RET,0x14F738EA+1
        defadr   DB_PATCH1_RET,0x142C7F1C+1
        defadr   DB_PATCH2_RET,0x142C85DC+1
        defadr   DB_PATCH3_RET,0x14F10B3C+1
        defadr   DB_PATCH4_RET,0x14F37810+1
        defadr   MESS_HOOK_RET,0x10129BBC+1

        defadr  memalloc,0x4BA32698
        defadr  memfree,0x4BA326C0
        defadr  Timer_Set,0x1449FF71

// --- Patch Keyhandler ---
	EXTERN Keyhandler_Hook
	RSEG  PATCH_KEYHANDLER1
        RSEG  CODE
        CODE16
NEW_KEYHANDLER1:
	MOV	R3, R6
	MOV	R7, R0
	MOV	R2, #0x1
	LDR	R1, [SP,#0]
	MOV	R0, R4
	BLX	Keyhandler_Hook
	MOV	R4, R0
	MOV	R2, R7
	MOV	R3, #0x1
	MOV	R1, R4
	MOV	R0, R6
	LDR	R7, =SFE(PATCH_KEYHANDLER1)+1
	BX	R7

	RSEG  PATCH_KEYHANDLER1
        CODE16
        LDR     R2,=NEW_KEYHANDLER1
        BX      R2


	RSEG  PATCH_KEYHANDLER2
        RSEG  CODE
        CODE16
NEW_KEYHANDLER2:
	MOV	R3, R7
	PUSH	{R0,R1}
	MOV	R2, R1
	LDR	R1, [SP,#8]
	ADD	R0, SP,#0xC
	LDRH	R0, [R0,#0x8]
	BLX	Keyhandler_Hook
	POP	{R2,R3}
	ADD	R1, SP,#0x4
	STRH	R0, [R1,#0x8]
	MOV	R1, R0
	LDR	R0, =SFE(PATCH_KEYHANDLER2)+1
	BX	R0

	RSEG  PATCH_KEYHANDLER2
        CODE16
        LDR     R3,=NEW_KEYHANDLER2
        BX      R3


	RSEG  PATCH_KEYHANDLER3
        RSEG  CODE
        CODE16
NEW_KEYHANDLER3:
	MOV	R3, R7
	PUSH	{R0,R1}
	MOV	R2, R1
	LDR	R1, [SP,#8]
	LDRH	R0, [R4,#0]
	BLX	Keyhandler_Hook
	MOV	R2, SP
	STRH	R0, [R2,#16]
	MOV	R1, R0
	MOV	R0, R7
	LDR	R2, =SFE(PATCH_KEYHANDLER3)+1
	MOV	R12, R2
	POP	{R2,R3}
	BX	R12

	RSEG  PATCH_KEYHANDLER3
        CODE16
        LDR     R3,=NEW_KEYHANDLER3
        BX      R3


	RSEG  PATCH_KEYHANDLER3_NOP(1)
        CODE16
	NOP


	RSEG  PATCH_KEYHANDLER3_CHANGE1(1)
        CODE16
	MOV	R2, SP
	LDRH	R0, [R2,#4]


	RSEG  PATCH_KEYHANDLER3_CHANGE2(1)
        CODE16
	MOV	R3, SP
	LDRH	R0, [R3,#4]


// --- CreateLists ---

        EXTERN  CreateLists
        RSEG  CODE
        CODE16
PATCH_STANDBY:

        STR     R0, [R7,#0xC]
        STR     R0, [R7,#0x10]
        STR     R0, [R7,#0x14]
        STR     R0, [R7,#0x18]
        BLX     CreateLists
        MOV     R0, #0
        LDR     R5,=STANDBY_RET
        BX      R5

        RSEG  PATCH_STANDBY_CALL
        CODE16
        LDR     R5,=PATCH_STANDBY
        BX      R5


// --- ParseHelperMessage ---
        EXTERN  ParseHelperMessage
        RSEG   CODE
        CODE16
MESS_HOOK:
	MOV	R7, #1
        LDR     R5, [R6, #0]

        BLX     ParseHelperMessage
        LDR     R3, =MESS_HOOK_RET
        BX      R3

        RSEG   PATCH_MMI_MESSAGE_HOOK
        CODE16
        LDR     R7,=MESS_HOOK
        BX      R7

// --- PageAction1 ---
        EXTERN  PageAction_Hook2
        RSEG    PATCH_PageActionImpl_All
        RSEG   CODE
        CODE16
PG_ACTION:
	MOV	R2, R5
	LDR	R1, [SP,#0x14]
	MOV	R0, R4
        BLX     PageAction_Hook2
        LDR     R7,=SFE(PATCH_PageActionImpl_All)+1
        BX      R7



        RSEG    PATCH_PageActionImpl_All
        CODE16
        LDR     R2, =PG_ACTION
        BX      R2


        EXTERN  PageAction_Hook2
        RSEG    PATCH_PageActionImpl_EnterExit
        RSEG   CODE
        CODE16
PG_ACTION2:
	LDR	R2, [SP,#0x1C]
	LDR	R1, [SP,#0x24]
	MOV	R0, R5
        BLX     PageAction_Hook2
        LDR     R7,=SFE(PATCH_PageActionImpl_EnterExit)+1
        BX      R7



        RSEG    PATCH_PageActionImpl_EnterExit
        CODE16
        LDR     R2, =PG_ACTION2
        BX      R2

// --- Data Browser ---

        EXTERN  GetExtTable
        RSEG   CODE
        CODE16
DB_PATCH1:
        BLX     GetExtTable
        LSL     R1, R4, #2
        LDR     R0, [R0,R1]
        LDR     R1, =0x15B5CE04
        LDR     R3, =DB_PATCH1_RET
        BX      R3


        RSEG   CODE
        CODE16

DB_PATCH2:

        BLX     GetExtTable
	LSL	R1, R7, #2
	LDR	R0, [R0,R1]
	LDR	R1, =0x15B5CE04
        LDR     R3, =DB_PATCH2_RET
        BX      R3

        RSEG   CODE
        CODE16

DB_PATCH3:
        BLX     GetExtTable
	LSL	R1, R5, #2
	LDR	R7, [R0,R1]
	LDR	R0, =0x15B5CE04
        LDR     R3, =DB_PATCH3_RET
        BX      R3


        RSEG   CODE
        CODE16

DB_PATCH4:
        BLX     GetExtTable
	LSL	R1, R5, #2
	LDR	R0, [R0,R1]
	LDR	R1, =0x15B5CE04
	STR	R0, [SP,#0]
        LDR     R3, =DB_PATCH4_RET
        BX      R3



        RSEG   PATCH_DB1(2)
        CODE16
        LDR    R3, =DB_PATCH1
        BX     R3

        RSEG   PATCH_DB2(2)
        CODE16
        LDR    R3, =DB_PATCH2
        BX     R3

        RSEG   PATCH_DB3(2)
        CODE16
        LDR    R3, =DB_PATCH3
        BX     R3

        RSEG   PATCH_DB4(2)
        CODE16
        LDR    R3, =DB_PATCH4
        BX     R3

        END
