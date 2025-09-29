//K510_R4EA031
        RSEG   CODE
        CODE32

TABMENUBAR_NAME EQU 0x448ADD48

defadr  MACRO   a,b
        PUBLIC  a
a       EQU     b
        ENDM

        defadr  memalloc,0x4535D930+1
        defadr  memfree,0x4535E664+1

// --- Patch Keyhandler ---
        EXTERN  Keyhandler_Hook
        RSEG    PATCH_KEYHANDLER1
        RSEG    CODE
        CODE16
NEW_KEYHANDLER1:
        MOV     R3, R6
        PUSH	{R0,R1}
        MOV     R2, R0
        LDRH	R0, [R4,#0]
        BLX	Keyhandler_Hook
        MOV     R2, SP
        STRH	R0, [R2,#0xC]
        MOV	R1, R0
        LDR	R2, =SFE(PATCH_KEYHANDLER1)+1
        MOV 	R12, R2
        POP 	{R2,R3}
        BX	R12

        RSEG  PATCH_KEYHANDLER1
        CODE16
        LDR     R3,=NEW_KEYHANDLER1
        BX      R3


        RSEG    PATCH_KEYHANDLER1_STACK1(1)
        CODE16
        SUB	SP, #0x8


        RSEG    PATCH_KEYHANDLER1_STACK2(1)
        CODE16
        ADD	SP, #0x8


        RSEG    PATCH_KEYHANDLER1_CHANGE1(1)
        CODE16
        MOV	R0, SP
        LDRH    R1, [R0,#0xC]


        RSEG    PATCH_KEYHANDLER1_CHANGE2(1)
        CODE16
        MOV	R0, SP
        LDRH	R1, [R0,#0x8]


        RSEG    PATCH_KEYHANDLER1_CHANGE3(1)
        CODE16
        MOV     R6, SP
        LDRH	R2, [R6,#0x4]


        RSEG    PATCH_KEYHANDLER1_CHANGE4(1)
        CODE16
        MOV     R3, SP
        LDRH    R0, [R3,#0x4]


	RSEG    PATCH_KEYHANDLER2
        RSEG    CODE
        CODE16
NEW_KEYHANDLER2:
        MOV     R3, R5
        MOV     R7, SP
        PUSH	{R0}
        MOV     R2, R0
        MOV     R1, #0x0
        LDRH	R0, [R7,#0x4]
        BLX     Keyhandler_Hook
        STRH	R0, [R7,#0x4]
        POP     {R2}
        MOV     R1, R0
        MOV     R3, #0x0
        LDR     R0, =SFE(PATCH_KEYHANDLER2)+1
        BX      R0

	RSEG    PATCH_KEYHANDLER2
        CODE16
        LDR     R2,=NEW_KEYHANDLER2
        BX      R2


	RSEG    PATCH_KEYHANDLER3
        RSEG    CODE
        CODE16
NEW_KEYHANDLER3:
        MOV     R3, SP
        PUSH	{R0,R1}
        MOV     R2, R0
        LDRH	R0, [R3,#0x4]
        MOV     R3, R7
        BLX     Keyhandler_Hook
        POP     {R2,R3}
        MOV     R1, SP
        STRH	R0, [R1,#0x0]
        MOV     R1, R0
        LDR     R0, =SFE(PATCH_KEYHANDLER3)+1
        BX      R0

	RSEG  PATCH_KEYHANDLER3
        CODE16
        LDR     R3,=NEW_KEYHANDLER3
        BX      R3


// --- ParseHelperMessage ---
        EXTERN  ParseHelperMessage
        RSEG    CODE
        CODE16
MESS_HOOK:
        LDR     R7, [SP,#0]
        LDR     R2, [R7,#0]
        STR     R2, [SP,#0x18]
        MOV     R3,LR
        ADD     R3,R3,#6
        PUSH    {R3}
        BLX     ParseHelperMessage
        POP     {PC}

        RSEG    PATCH_MMI_MESSAGE_HOOK
        CODE16
        LDR     R3,=MESS_HOOK
        BLX     R3


// --- PageAction1 ---
        EXTERN  PageAction_Hook2
        RSEG    PATCH_PageActionImpl_All
        RSEG   CODE
        CODE16
PG_ACTION:
        MOV     R2, R6
        MOV     R1, R5
        MOV     R0, R4
        BLX     PageAction_Hook2
        MOV     R4, R0
        LDR     R5,=SFE(PATCH_PageActionImpl_All)+1
        BX      R5

        RSEG    PATCH_PageActionImpl_All
        CODE16
        LDR     R2, =PG_ACTION
        BX      R2


        EXTERN  PageAction_Hook2
        RSEG    PATCH_PageActionImpl_EnterExit
        RSEG    CODE
        CODE16
PG_ACTION2:
        LDR     R2, [SP,#0x18]
        LDR     R1, [SP,#0x1C]
        MOV     R0, R6
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
        PUSH    {R3,LR}
        LSL     R4, R7, #2
        BLX     GetExtTable
        LDR     R4, [R0,R4]
        POP     {R0,PC}

        RSEG   CODE
        CODE16
DBEXT:
        PUSH    {LR}
        BLX     GetExtTable
        SUB     R5,R0,#4
        POP     {PC}

        RSEG    PATCH_DB1(1)
        CODE16
        BL       DB_PATCH

        RSEG    PATCH_DB2
        CODE16
        BL       DBEXT


        RSEG   CODE
        CODE16
TabMenuCheck:
        PUSH    {LR}
        LDR     R0, [R0,#0] //GUIObject_GetDispObject
        LDR     R0, [R0,#8] //DispObject_GetName ptr1
        LDR     R0, [R0,#0] //DispObject_GetName ptr2
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
