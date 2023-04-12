global main
extern printf
extern scanf

section .data
inpMessage db 'Enter an Integer value:',0
outputMessage db 'Output:%d',0
true equ 1
false equ 0
intFormat db "%d",0
section .text
main:
push rbp
mov rbp, rsp
sub rsp, 280

leave
ret
