STR:    .string  "abcd"
MAIN:   add  r3, LIST 
LOOP:   prn  #48
        lea  STR, r6
LIST:   .data  6, -9
        macr m_macr
        cmp  r3, #-6
        bne  END 
        endmacr
        inc  r6
        mov *r6, K
        sub  r1, r4

        .data -100  

        m_macr
K:      .data 31
        dec  K
        jmp  LOOP
END:    stop
