include "x.inc"
code16


freespace = 0x15E8EAC0

CClhServer_ICBCallManager_OnSetupPresent = 0x14F09A68
CLH_Server_GetData = 0x1428BF58
CLH_Wrapper_ICallManager_RejectSetup = 0x154F8D98

BookObj_FindBook = 0x1416E970

strcmp = 0x14D3B45C


BlackListBook_Name EQU db 'Black List',0


org 0x15BB6100
dw new_OnSetupPresent+1


org freespace
new_OnSetupPresent:

STACK_BUF = 0xC
Cli_STACK_OFFS = 0x0
CallType_STACK_OFFS = 0x4
CallId_STACK_OFFS = 0x8
ClientData_STACK_OFFS = 0x20

push {r1-r7,lr}
	sub sp, sp,STACK_BUF
	mov r4, r0
	adr r0, FindBook_BlackListBook
	add r0, 0x1
	ldr r1, _BookObj_FindBook
	blx r1
	mov r5, r0
	beq book_not_found

;fill functions array
	mov r0, sp
	ldr r1, _CClhServer_ICBCallManager_OnSetupPresent
	str r1, [r0]
	ldr r1, _CLH_Server_GetData
	str r1, [r0,0x4]
	ldr r1, _CLH_Wrapper_ICallManager_RejectSetup
	str r1, [r0,0x8]

	ldr r2, [sp,STACK_BUF+ClientData_STACK_OFFS]
	ldr r1, [sp,STACK_BUF+CallId_STACK_OFFS]
	ldr r0, [sp,STACK_BUF+CallType_STACK_OFFS]
	push {r0-r2}
	mov r2, r4
	ldr r3, [sp,STACK_BUF+Cli_STACK_OFFS+0xC]
	add r1, sp,0xC				;functions array pointer
	mov r0, r5
	ldr r4, [r5,0x18]	
	blx r4
	add sp, sp,STACK_BUF+0xC
pop {r1-r7,pc}


book_not_found:
	ldr r0, [sp,STACK_BUF+ClientData_STACK_OFFS]
	push {r0}
	mov r0, r4
	ldr r1, [sp,STACK_BUF+Cli_STACK_OFFS+0x4]
	ldr r2, [sp,STACK_BUF+CallType_STACK_OFFS+0x4]
	ldr r3, [sp,STACK_BUF+CallId_STACK_OFFS+0x4]
	ldr r4, _CClhServer_ICBCallManager_OnSetupPresent
	blx r4
	add sp, sp,STACK_BUF+0x4
pop {r1-r7,pc}


align 4
FindBook_BlackListBook:
push {lr}
	ldr r0, [r0]
	ldr r0, [r0,0x4]
	adr r1, book_name
	ldr r2, _strcmp
	blx r2
	cmp r0, 0x0
	beq book_found
	mov r0, 0x0
pop {pc}

book_found:
	mov r0, 0x1
pop {pc}



align 4
_BookObj_FindBook: dw BookObj_FindBook+1

_CClhServer_ICBCallManager_OnSetupPresent: dw CClhServer_ICBCallManager_OnSetupPresent+1
_CLH_Server_GetData: dw CLH_Server_GetData+1
_CLH_Wrapper_ICallManager_RejectSetup: dw CLH_Wrapper_ICallManager_RejectSetup+1

_strcmp: dw strcmp+1



align 4
book_name: BlackListBook_Name