; Error: Label starting with a number
1INVALIDLABEL: add r3, r4

; Error: Duplicate label definition
DUPLICATE: sub r5, r6
DUPLICATE: mul r7, r8

; Error: Invalid characters in label
INVALID@LABEL: div r1, r2

; Error: Label too long (assuming a maximum length of 31 characters)
THISLABELISWAYTOOOLONGANDSHOULDERROR: mov r0, r1

; Error: Missing colon after label
MISSINGCOLON mov r2, r3

; Error: Invalid directive
.invalid 5, 10, 15

; Error: Invalid operand for .data directive
.data hello

; Error: Invalid operand for .string directive
.string 12345

; Error: Missing operand for .entry directive
.entry

; Error: Missing operand for .extern directive
.extern

; Error: Use of undefined label
mov r1, UNDEFINEDLABEL

; Error: Invalid register number
mov r9, r2

; Error: Invalid addressing mode
mov #LABEL, r3

; Error: Too many operands
add r1, r2, r3

; Error: Too few operands
sub r1

; Error: Invalid instruction name
invalidinstruction r1, r2


; Valid data and string directives (for comparison)
.data 100, -42, 75
.string "This is a valid string"


; Error: Label starts with a reserved word
movLABEL: cmp r1, r2

