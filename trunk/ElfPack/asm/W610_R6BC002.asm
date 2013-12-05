//W610 R6BC002
#include "target.h"
        RSEG   CODE
        CODE32

TABMENUBAR_NAME EQU 0x45A56EEC

defadr  MACRO   a,b
        PUBLIC  a
a       EQU     b
        ENDM

        RSEG  CODE
        defadr   STANDBY_RET,0x452B89C4+1
        defadr   DB_PATCH_RET,0x44EB7864+1
        defadr   DB_EXT_RET,0x44EB7650+1
        defadr   DB_PATCH3_RET,0x44EB74F0+1
        defadr   DB_PATCH4_RET,0x44EB7EB8+1
        defadr   MESS_HOOK_RET,0x447AC644+1
	defadr   KEY_HOOK_REPEAT_RETUN,0x4534DA52+1
	defadr   KEY_HOOK_TIMER_RETUN,0x4534DAAE+1
	defadr   KEY_HOOK_TIMER_RETUN_NE,0x4534DA78+1

        defadr  memalloc,0x44D4C404+1
        defadr  memfree,0x44D4C430+1
        defadr  Timer_Set,0x452A27ED

// --- Patch Keyhandler ---
	EXTERN Keyhandler_Hook
	RSEG  PATCH_KEYHANDLER1
        RSEG  CODE
        CODE16
NEW_KEYHANDLER1:
	MOV	R3, R7
	PUSH	{R0,R5}
	MOV	R1, R5
	LDRH	R0, [R4,#0]
	BLX	Keyhandler_Hook
	LDR	R1, =KEY_LAST
	CMP	R1, R0
	BEQ	GO_TO_TIMER
	STRH	R0, [R4,#0]
	MOV	R1, R0
	MOV	R0, R7
	LDR	R2, =SFE(PATCH_KEYHANDLER1)+1
	MOV	R12, R2
	POP	{R2,R3}
	BX	R12

GO_TO_TIMER:
	ADD	SP, #0x8
	LDR	R0, =KEY_HOOK_REPEAT_RETUN
	BX	R0


	RSEG  PATCH_KEYHANDLER1
        CODE16
        LDR     R3,=NEW_KEYHANDLER1
        BX      R3


	RSEG  PATCH_KEYHANDLER2
        RSEG  CODE
        CODE16
NEW_KEYHANDLER2:
	MOV	R3, R6
	PUSH	{R0,R7}
	MOV	R2, R0
	MOV	R1, R7
	MOV	R0, R4
	BLX	Keyhandler_Hook
	MOV	R4, R0
	POP	{R2,R3}
	MOV	R1, R4
	MOV	R0, R6
	LDR	R7, =SFE(PATCH_KEYHANDLER2)+1
	BX	R7

	RSEG  PATCH_KEYHANDLER2
        CODE16
        LDR     R2,=NEW_KEYHANDLER2
        BX      R2


	RSEG  PATCH_KEYHANDLER3
        RSEG  CODE
        CODE16
NEW_KEYHANDLER3:
	MOV	R3, R6
	MOV	R7, SP
	PUSH	{R0,R5}
	MOV	R2, R0
	MOV	R1, R5
	LDRH	R0, [R7,#0x0]
	BLX	Keyhandler_Hook
	POP	{R2,R3}
	STRH	R0, [R7,#0x0]
	MOV	R0, #0x0
	LDR	R1, =SFE(PATCH_KEYHANDLER3)+1
	BX	R1

	RSEG  PATCH_KEYHANDLER3
        CODE16
        LDR     R3,=NEW_KEYHANDLER3
        BX      R3


	RSEG  PATCH_KEYHANDLER4
        RSEG  CODE
        CODE32
NEW_KEYHANDLER4:

	MOV	R2, R4
	BLX	Timer_Set
	STRH	R0, [R4,#16]
	LDRH	R0, [R4,#0]
	LDR	R1, =KEY_LAST
	CMP	R0, R1
	LDRNE	R0, =KEY_HOOK_TIMER_RETUN_NE
	BXNE	R0
	LDR	R0, =KEY_HOOK_TIMER_RETUN
	BX	R0


	RSEG  PATCH_KEYHANDLER4
        CODE16
        LDR     R2,=NEW_KEYHANDLER4
        BX      R2


// --- CreateLists ---

        EXTERN  CreateLists
        RSEG  CODE
        CODE16
PATCH_STANDBY:

        STR     R0, [R5,#8]
        STR     R0, [R5,#0]
        STR     R0, [R5,#0xC]
        STR     R0, [R5,#0x10]
        BLX     CreateLists
        MOV     R0, #0
        LDR     R1,=STANDBY_RET
        BX      R1

        RSEG  PATCH_STANDBY_CALL
        CODE16
        LDR     R1,=PATCH_STANDBY
        BX      R1


// --- ParseHelperMessage ---
        EXTERN  ParseHelperMessage
        RSEG    CODE
        CODE16
MESS_HOOK:
        LDR     R3, [R0, #0]
        STR     R3, [SP,#0x1C]
        BLX     ParseHelperMessage
        LDR     R3, =MESS_HOOK_RET
        BX      R3

        RSEG   PATCH_MMI_MESSAGE_HOOK
        CODE16
        LDR     R3,=MESS_HOOK
        BX      R3

// --- PageAction1 ---
        EXTERN  PageAction_Hook2
        RSEG    PATCH_PageActionImpl_All
        RSEG   CODE
        CODE16
PG_ACTION:
	MOV	R2, R5
	LDR	R1, [SP,#0x18]
	MOV	R0, R4
        BLX     PageAction_Hook2
        LDR     R5,=SFE(PATCH_PageActionImpl_All)+1
        BX      R5



        RSEG    PATCH_PageActionImpl_All
        CODE16
        LDR     R2, =PG_ACTION
        BX      R2


        EXTERN  PageAction_Hook2
        RSEG    PATCH_PageActionImpl_EnterExit
        RSEG   CODE
        CODE16
PG_ACTION2:
	LDR	R2, [SP,#0x18]
	LDR	R1, [SP,#0x1C]
	MOV	R0, R6
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
DB_PATCH:
        LSL     R7, R1, #2
        BLX     GetExtTable
        LDR     R7, [R0,R7]
        LDR     R1, =0x45967D64
        LDR     R3, =DB_PATCH_RET
        BX      R3


        RSEG   CODE
        CODE16

DBEXT:

        ADD     R2, R2, #1
        ADD     R1, R5, #0
        ADD     R0, R4, #0
        PUSH    {R0-R2}
        BLX     GetExtTable
        ADD     R5, R0, #0
        POP     {R0-R2}
        LDR     R3, =DB_EXT_RET
        BX      R3

        RSEG   CODE
        CODE16

DB_PATCH3:
        MOV     R6, #0
        ADD     R5, R0, #0
        BEQ     L_DB3EX
        BLX     GetExtTable
        ADD     R4, R0, #0
        LDR     R1, =DB_PATCH3_RET
        BX      R1

L_DB3EX
        ADD     R0, R6, #0
        ADD     SP, #8
        POP     {R4-R7,PC}

        RSEG   CODE
        CODE16

DB_PATCH4:
        ADD     R2, SP, #4
        PUSH    {R2}
        LDR     R0, [R0, #0]
        MOV     R3, #2
        PUSH    {R0-R3}
        BLX     GetExtTable
        ADD     R7, R0, #0
        POP     {R0-R3}
        LDR     R1, =DB_PATCH4_RET
        BX      R1



        RSEG   PATCH_DB1(2)
        CODE16
        LDR    R3, =DB_PATCH
        BX     R3

        RSEG   PATCH_DB2(2)
        CODE16
        LDR    R3, =DBEXT
        BX     R3

        RSEG   PATCH_DB3(2)
        CODE16
        LDR    R3, =DB_PATCH3
        BX     R3

        RSEG   PATCH_DB4(2)
        CODE16
        LDR    R3, =DB_PATCH4
        BX     R3

        RSEG   CODE
        CODE16
TabMenuCheck:
        PUSH    {LR}
        LDR     R0, [R0, #0] //GUIObject_GetDispObject
        LDR     R0, [R0, #8] //DispObject_GetName ptr1
        LDR     R0, [R0, #0] //DispObject_GetName ptr2
        LDR     R1, =TABMENUBAR_NAME
        CMP     R0, R1
        BNE     TabMenuCheck_false
        MOV     R0, #1
        POP     {PC}
TabMenuCheck_false:
        MOV     R0, #0
        POP     {PC}


        RSEG   PATCH_TabMenuCheck
        CODE16
        LDR     R3, =TabMenuCheck
        BX      R3

        END
