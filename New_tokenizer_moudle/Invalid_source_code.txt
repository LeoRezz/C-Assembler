MAIN:	add  r3, LIST@            ; Invalid character in label
LOOP:	prn  #48.5                ; Decimal point in immediate value
	lea  STR r6                ; Missing comma between operands
	inc  r8                    ; Invalid register number
	mov *r6 K                  ; Missing comma between operands
	sub  r1, 4r                ; Inverted number and register
	cmp  r3, #--6              ; Double negative in immediate value
	bne  2END                  ; Label starting with a number
	dec  K+                    ; Invalid character after operand
	jmp  LOO P                 ; Space in label name
END:	sto p                     ; Misspelled instruction (space in middle)
STR:	.string  "ab\\\``   jhgfd(*&&*())``\!@##%#$%^%^&!\\\\""""\\\\cd\g"
LIST:	.data  6, -09             ; Leading zero in negative number
	.data -100,                ; Trailing comma
K:	.data  31H                 ; Invalid hexadecimal format