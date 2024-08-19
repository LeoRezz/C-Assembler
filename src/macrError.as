; This is a comment line and should be ignored

; Valid macro definition
macr valid_macro
    mov r1, r2
    add r3, #5
endmacr

; Error: Invalid macro name (reserved word)
macr mov
    sub r4, r5
endmacr

; Error: Macro definition with extra arguments
macr extra_arg_macro extra_arg
    cmp r1, #0
endmacr

; Error: Invalid macro name (starts with a number)
macr 1invalid_name
    jmp LOOP
endmacr

; Error: Invalid macro name (contains invalid character)
macr invalid@name
    dec r6
endmacr

; Valid macro usage
valid_macro

; Error: Macro usage with extra arguments
valid_macro extra_arg

; Error: Usage of undefined macro
undefined_macro

; Error: endmacr without corresponding macr
endmacr

; Normal assembly code
START: mov r0, #0
LOOP:  cmp r0, #10
       bne END
       inc r0
       jmp LOOP
END:   stop

; This should be treated as a normal instruction, not a macro invocation
mov r7, #100

.entry LOOP
.extern UNKNOWN_SYMBOL