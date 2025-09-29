	RSEG CODE:CODE:NOROOT(2)

	public	InterruptsAndFastInterrupts_Off
InterruptsAndFastInterrupts_Off:
	MRS	R0, CPSR
	ORR	R12, R0, #0xC0
	MSR	CPSR_c, R12
	ANDS	R0, R0, #0xC0
	BX	LR


	public	InterruptsAndFastInterrupts_Restore
InterruptsAndFastInterrupts_Restore:
	ANDS	R0, R0, #0xC0
	MRS	R12, CPSR
	BIC	R12, R12, #0xC0
	ORR	R12, R12, R0
	MSR	CPSR_c, R12
	BX	LR


	public	cp15_write_DAC
cp15_write_DAC:
	MRC	p15, 0, R1, c3, c0, 0
	MCR	p15, 0, R0, c3, c0, 0
	MOV	R0, R1
	BX	LR

	end