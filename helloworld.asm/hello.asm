;arm

;пример работающего эльфа на асме
;что-то серьёзное делать не получится, fasmarm не поддерживает релокейшны

        include "%fasminc%\x.inc"

; тип выходного файла
USEDWARF = 1

if USEDWARF = 1

        format	ELF DWARF executable
        section	'main' executable readable writeable align 16
else
        format	ELF executable
        segment	executable readable writeable

end if
        entry 	main
        code32
ELF_BEGIN:

main:
        stmfd   sp!,{r1-r7,lr}

        adr     r0, .hello
        mov     r1, 0
        ldr     r2, .SID_ANY_LEN
        swi     0x15D ;TextID_Create(const void * wstr , int  flag , int len);

        mov     r1, r0
        ldr     r0, .header
        ldr     r2, .icon
        mov     r3, 1
        mov     r4, 0
        stmfd   sp!, {r4}
        stmfd   sp!, {r4}
        swi     0x257 ;MessageBox
        add     sp, sp, 8

        ;SUBPROC(elf_exit)
        adr     r0, elf_exit
        swi     0x108

        ;return 0
        mov     r0,0
        ldmfd   sp!, {r1-r7,pc}

.SID_ANY_LEN:   dw 0xFFFF
.icon:	dw 0xFFFF
.header:dw 0x6FFFFFFF
.hello: du	'Hello world', 0

align 4
elf_exit:
        STMFD   SP!, {LR}
        swi     0x8104 ;mfree_adr()
        mov     r1, r0
        adr     r0, ELF_BEGIN
        LDMFD   SP!, {LR}
        bx      r1


if USEDWARF = 1

        section '.dynconst' readable align 4
        ; сюда по идее константы нужно класть
        section '.dynconstcnt' readable align 4
        dw	0 ;число констант

        section '.dynpage' readable align 4
        ; сюда по идее страницы с константами нужно класть
        section '.dynpagecnt' readable align 4
        dw	0 ;число страниц

else
	;тут тоже константы со старницами, но не отдельными сегментами
        dw	0
        dw	0

end if
