     AREA prog, CODE, READWRITE
     ENTRY
     MOV r0,#51
     MOV r1,#34
GCD  CMP r0, r1
	 SUBGT r0, r0, r1
	 SUBLT r1, r1, r0
	 BNE GCD
Loop B Loop
     END