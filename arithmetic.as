.entry START
.extern calculate

        macr print_str
        lea STR, r3
        mov *r3, r4
        cmp r4, #0
        bne START
        endmacr

START:  mov #5, r1
        mov #10, r2
        add r1, r2
        jsr calculate
        print_str
        stop
STR:    .string "Result calculated!"
DATA:   .data 1, 2, 3, 4, 5