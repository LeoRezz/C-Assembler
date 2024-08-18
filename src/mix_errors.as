; 1. Reserved words used as label definitions
mov: add r1, r2
.data: sub r3, r4

; 5. Invalid register numbers (must be between 0 and 7)
    mov *r8, r1
    add r2, r9

; 6. Immediate values out of range (must be between -2048 and 2047)
    mov #-2049, r1
    add r2, #2048

; 7. Invalid addressing modes for specific instructions
    lea #5, r1        
; lea doesn't support immediate addressing for source
    mov r1, #100      
; mov doesn't support immediate addressing for destination

; 8. Incorrect number of operands for instructions
    add r1            
; Too few operands
    sub r1, r2, r3    
; Too many operands

; 9. Invalid arguments for directives
    .data "hello"     
; .data expects numbers, not strings
    .string 123       
; .string expects a string, not a number

; 10. Missing arguments for directives
    .data
    .string

; 11. Use of external labels in branching instructions
    .extern EXTERNAL
    bne EXTERNAL    
; Can't use external labels for branching

; Valid instructions for comparison
VALIDLABEL: mov r1, r2
    add #1000, r3
    lea VALIDLABEL, r4
    .data 6, -7, 100
    .string "This is a valid string"

; Additional test cases
    cmp r0, #-2048    
; Edge case: minimum immediate value
    add r7, #2047     
; Edge case: maximum immediate value
    .entry VALIDLABEL
    jmp VALIDLABEL