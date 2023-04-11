global main
extern printf
extern scanf

section .data
inpMessage db 'Enter an Integer value:',0
outputMessage db 'Output:%d',0
intFormat db "%d",0
section .text
main:
push rbp
mov rbp, rsp
sub rsp, 144
mov rax, 5
mov QWORD[rbp-8-48], rax
mov rax, 9
mov QWORD[rbp-8-64], rax
mov rdi, inpMessage
mov rax, 0
call printf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-0]
mov rsi, rax
call scanf WRT ..plt
mov rdi, inpMessage
mov rax, 0
call printf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-16]
mov rsi, rax
call scanf WRT ..plt
movsxd rax, DWORD[rbp-8-64]
movsxd rbx, DWORD[rbp-8-64]
imul rax, rbx
mov QWORD[rbp-8-32], rax
movsxd rax, DWORD[rbp-8-64]
movsxd rbx, DWORD[rbp-8-64]
sub rax, rbx
mov DWORD[rbp-8-80], eax
movsxd rax, DWORD[rbp-8-16]
movsxd rbx, DWORD[rbp-8-16]
imul rax, rbx
mov QWORD[rbp-8-64], rax
movsxd rax, DWORD[rbp-8-48]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-112], rax
movsxd rax, DWORD[rbp-8-0]
movsxd rbx, DWORD[rbp-8-0]
imul rax, rbx
mov QWORD[rbp-8-128], rax
movsxd rax, DWORD[rbp-8-128]
movsxd rbx, DWORD[rbp-8-128]
sub rax, rbx
mov DWORD[rbp-8-96], eax
movsxd rax, DWORD[rbp-8-96]
movsxd rbx, DWORD[rbp-8-96]
add rax, rbx
mov QWORD[rbp-8-48], rax
movsxd rax, DWORD[rbp-8-48]
movsxd rbx, DWORD[rbp-8-48]
add rax, rbx
mov QWORD[rbp-8-16], rax
movsxd rax, DWORD[rbp-8-16]
movsxd rbx, DWORD[rbp-8-16]
add rax, rbx
mov QWORD[rbp-8-0], rax
movsxd rax, DWORD[rbp-8-0]
mov QWORD[rbp-8-32], rax
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-32]
mov rax,0
call printf WRT ..plt
leave
ret
