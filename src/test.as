THISLABELISWAYTOOOLONGANDSHOULDERROR: mov r0, r1
; Error: Label starting with a number
1INVALIDLABEL: add r3, r4
; Error: Duplicate label definition
DUPLICATE: sub r5, r6
DUPLICATE: add r7, r8
; Error: Label too long (assuming a maximum length of 31 characters)
THISLABELISWAYTOOOLONGANDSHOULDERROR: mov r0, r1