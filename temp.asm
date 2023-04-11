global main
extern printf
extern scanf

section .datainpMessage db 'Enter an Integer value:
',0section .text
main:
movsxd rax, 5
movsxd DWORD[rbp-8--1], rax
movsxd rax, 9
movsxd DWORD[rbp-8--1], rax
mov rdi, inpMessage
mov rax, 0
call printf
mov rdi, intFormat
lea rax, DWORD[rbp-8--1]
mov rsi, rax
call scanf
mov rdi, inpMessage
mov rax, 0
call printf
mov rdi, intFormat
lea rax, DWORD[rbp-8--1]
mov rsi, rax
call scanf
imul eax, ebx
movsxd DWORD[rbp-8-4], eax
sub eax, ebx
movsxd DWORD[rbp-8-10], eax
imul eax, ebx
movsxd DWORD[rbp-8-8], eax
movsxd eax, DWORD[rbp-8-6]
mov ebx, 2
imul eax, ebx
movsxd DWORD[rbp-8-14], eax
imul eax, ebx
movsxd DWORD[rbp-8-16], eax
sub eax, ebx
movsxd DWORD[rbp-8-12], eax
add eax, ebx
movsxd DWORD[rbp-8-6], eax
add eax, ebx
movsxd DWORD[rbp-8-2], eax
add eax, ebx
movsxd DWORD[rbp-8-0], eax
movsxd rax, temp1
movsxd DWORD[rbp-8--1], rax
mov rdi, DWORD[rbp-8--1]mov rax,0call printf
