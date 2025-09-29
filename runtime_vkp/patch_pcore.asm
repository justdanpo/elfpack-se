	RSEG CODE:CODE:NOROOT(2)
	CODE32

	extern	check_static_after_map

	public	patch_pcore_static_cache
patch_pcore_static_cache:
#if defined(DB3350) || defined(DB3150_R1)
	MOV	R0, R5
	MOV	R1, R6
#else
	MOV	R0, R4
	MOV	R1, R7
#endif
	BL	check_static_after_map
	LDR	R6, _patch_pcore_static_cache_return_addr
	BX	R6

_patch_pcore_static_cache_return_addr DCD 0x12345678


	extern	check_nonstatic_after_map

	public	patch_pcore_nonstatic_cache
patch_pcore_nonstatic_cache:
#if defined(DB3350)
	ADD	R1, R0, R1
	ADD	R2, SP, #0x10
	LDR	R0, [SP,#0xC]
#elif defined(DB3150_R1)
	ADD	R1, R12, R0, LSL #10
	MOV	R0, R6
	ADD	R2, SP, #0xC
#else
	ADD	R1, R12, R0, LSL #10
	MOV	R0, R5
	ADD	R2, SP, #0xC
#endif
	BL	check_nonstatic_after_map
	LDR	R1, _patch_pcore_nonstatic_cache_return_addr
	BX	R1

_patch_pcore_nonstatic_cache_return_addr DCD 0x87654321

	end