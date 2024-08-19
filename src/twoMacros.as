	macr macr1
	cmp  r3, #-6
	bne  END 
	endmacr
	macr macr2
	add  r0, r5
	endmacr
	macr1
	macr2
	macr1
	macr2
END:	stop
STR:	.string  "abcd"
LIST:	.data  6,-9
	.data -100
K:	.data  31