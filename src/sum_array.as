.entry MAIN
.extern SUBTRACT

        macr decrement
        sub COUNT, #1
        endmacr

        macr sum_arr
        lea NUMBERS, r0
        mov *r0, r1
        add SUM, r1
        inc r0
        endmacr

MAIN:   mov #5, COUNT
        mov #0, SUM
LOOP:   cmp COUNT, #0
        beq END
        sum_arr
        decrement
        jmp LOOP
END:    mov SUM, r0
        jsr SUBTRACT
        stop
NUMBERS:.data 10, 20, 30, 40, 50
COUNT:  .data 5
SUM:    .data 0