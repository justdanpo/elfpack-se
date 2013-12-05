        NAME    main

;pLIB_TOP equ 0x4c000000

        PUBLIC  main
        RSEG   PATCH_SWI
main:
	LDR	PC,_HUNKADR
_HUNKADR
	DCD	_HUNK1
SWI_RET



;======================================
	RSEG   CODE
	extern	elfpackdata
_HUNK1
	TST     R0, #0x20
	BNE	_RETURN
	LDR	R0,[LR,#-4]
	BIC	R0,R0,#0xFF000000
	CMP	R0,#255
	BLS	_RETURN
	CMP	R0,#0x100
	BEQ	IMB
	STMFD	SP!,{R1-R3}
	LDR	R2,=elfpackdata
	LDR	R2,[R2]
	CMP	R2,#0
	LDRNE	R1,[R2,#44]		; Указатель на таблицу адресов
	CMPNE	R1,#0
	LDREQ	R1,=Library
	SUB	R1,R1,#0x400
	ADR	R2,arm_jumper		; Берем адрес джампера
	BIC	R3,R0,#0x8000
	CMP	R3,#4096
	BHI	exit
	TST	R0,#0x8000		; А не адрес нам надо получить?
	STMEQFD	SP!,{LR}		; Копируем адрес возврата из LR_svc только если вызов функции
	LDMEQFD	SP!,{LR}^		; в LR_usr, он будет использован вызываемой функцией
	LDR	R12,[R1,R3,LSL#2]	; Берем адрес функции
	STRNE	R12,[SP,#0x14]		; пишем адрес в R0(стек)
	BNE	exit

//	CMP	R12,#0xFFFFFFFF
//	LDREQ	R2,=FUNC_ABORT
//	STREQ	R0,[SP,#0x14]

	STR	R2,[SP,#0x18]		; Пишем адрес джампера для возврата в стеке (PC)
exit:
	LDMFD	SP!,{R1-R3}
exit1:
	LDR	R0,[SP],#8
	MSR	SPSR_cxsf,R0
//        LDR     R0,[SP],#4
//        LDR     LR,[SP],#4
//        MOVS    PC,LR
	LDMFD	SP!,{R0,PC}^
arm_jumper:
	BX	R12
;
_RETURN
	LDR	R0,[SP]
	AND     R0, R0, #0x80
	ORR     R0, R0, #0x13
	LDR	PC,=SWI_RET

IMB:
	MRS	R1,CPSR		; Запрещаем прерывания
	ORR	R1,R1,#0xC0
	MSR	CPSR_c,R1
	NOP
	NOP
	NOP
;Выполняем необходимую требуху с кешем
#ifndef K600_R2BB001
clean_loop:
	MRC 	p15, 0, r15, c7, c10, 3 ; clean entire dcache using test and clean
	NOP
	NOP
	NOP
	BNE	clean_loop
#endif
	MOV	R0,#0
	MCR	p15, 0, r0, c7, c10, 4 ; drain write buffer
	NOP
	NOP
	NOP
	NOP
	MCR	p15, 0, r0, c7, c5, 0 ; invalidate icache
	NOP
	NOP
	NOP
	NOP
;Выходим
        B       exit1





pLIB_TOP:
        DC32  0

//        EXTERN  GetUserData
//        EXTERN  RemoveUserData
        EXTERN  malloc
        EXTERN  mfree
        EXTERN  GetDir
        EXTERN  fopen
        EXTERN  ModifyKeyHook
	EXTERN	SUBPROCExec
	EXTERN	MMIPROCExec
        EXTERN  ModifyOSEHook
//        EXTERN  ELF_RemoveFromList
        EXTERN  ModifyUIHook
        EXTERN  elfload_int

        EXTERN  LoadDLL_int
        EXTERN  UnLoadDLL_int

        EXTERN  ModifyUIHook1

        EXTERN getepd

	RSEG    LIB:DATA(2)
	PUBLIC	Library
Library:
        DC32    IMB
        DC32    0//GetUserData
        DC32    0//RemoveUserData
        DC32    malloc

        DC32    mfree
        DC32    GetDir
        DC32    fopen
        DC32    ModifyKeyHook

	DC32	SUBPROCExec
        DC32    MMIPROCExec
        DC32    0//ModifyOSEHook
        DC32    0//ELF_RemoveFromList

        DC32    0//ModifyUIHook
        DC32    elfload_int

        DC32    LoadDLL_int
        DC32    UnLoadDLL_int

        DC32    ModifyUIHook1

        END     main
