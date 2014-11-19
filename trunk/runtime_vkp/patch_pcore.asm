	RSEG CODE:CODE:NOROOT(2)
	CODE32
	
	extern	check_static_after_map

	public patch_pcore_static_cache
patch_pcore_static_cache:
	MOV	R0, R4
	MOV	R1, R7
	BL	check_static_after_map
	LDR	R6, _pcore_return_addr
	BX	R6
	
_pcore_return_addr DCD 0x12345678
	
	end