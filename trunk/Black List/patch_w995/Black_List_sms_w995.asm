include "x.inc"
code16


freespace = 0x10983000

BookObj_FindBook = 0x1416E970
strcmp = 0x14D3B45C

timer_sig = 0x00E01D00
Do_TimerSet_1_S = 0x4BAA23D4

MSG_SenReportError = 0x104D8110
MSG_unk = 0x1024588C

original_return = 0x102B14B4
filtered_return = 0x102B14E6


BlackListBook_Name EQU db 'Black List',0

BLOCK_SMS = 0x1
ALLOW_SMS = 0x0

DELIVER_TYPE = 0x0

ERROR_ID = 0x0



org 0x102B14B0
	bx r0
	nop

org 0x102B1538
dw filter_sms+1


org freespace
filter_sms:
	push {r1,r4-r7}
	ldr r0, [r5,0x4]
	ldrb r0, [r0]
	cmp r0, DELIVER_TYPE		;check pdu type
	bne original

	adr r0, FindBook_BlackListBook
	add r0, 0x1
	ldr r1, _BookObj_FindBook
	blx r1
	cmp r0, 0x0
	beq original

	mov r1, r6		;smsc*
	mov r2, r7		;pdu*
	ldr r3, [r0,0x24]
	blx r3
	cmp r0, ALLOW_SMS
	beq original
;block sms
	pop {r1,r4-r7}
	ldr r0, _timer_sig
	ldr r2, _Do_TimerSet_1_S
	blx r2
	ldr r0, [r5,4]
	mov r1, ERROR_ID
	ldr r3, _MSG_SenReportError
	blx r3
	ldr r0, [r5,4]
	ldr r3, _MSG_unk
	blx r3
	ldr r0, _filtered_return
	bx r0
	

original:
	pop {r1,r4-r7}
	ldr r0, _timer_sig
	ldr r2, _Do_TimerSet_1_S
	blx r2
	ldr r0, _original_return
	bx r0


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
_timer_sig: dw timer_sig
_original_return: dw original_return+1
_filtered_return: dw filtered_return+1

_MSG_SenReportError: dw MSG_SenReportError+1
_MSG_unk: dw MSG_unk+1

_Do_TimerSet_1_S: dw Do_TimerSet_1_S+1

_BookObj_FindBook: dw BookObj_FindBook+1
_strcmp: dw strcmp+1

align 4
book_name: BlackListBook_Name