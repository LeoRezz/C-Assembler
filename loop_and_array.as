.entry LOOP
.extern PRINT_RESULT

        macr inc_ptr
        inc r0
        inc r2
        endmacr

MAIN:   mov #0, r0
        mov #1, r1
        mov ARRAY, r2
LOOP:   cmp r0, #10
        beq END
        add r1, r2
        mov r1, *r2
        inc_ptr
        jmp LOOP
END:    jsr PRINT_RESULT
        stop
ARRAY:  .data 0, 0, 0, 0, 0, 0, 0, 0, 0, 0