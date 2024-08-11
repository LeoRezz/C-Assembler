LIST:   .data  6, -9
MAIN:   add  r3, LIST 
        .data -100
LOOP:   prn  #48
        lea  STR, r6
        macr m_macr
        cmp  r3, #-6
        bne  END 
        endmacr
        inc  r6
        mov *r6, K
        sub  r1, r4
        m_macr
        dec  K
        jmp  LOOP
STR:    .string  "abcd"
        .data 31
END:    stop
K:      .data 31