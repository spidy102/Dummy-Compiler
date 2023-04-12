global main
extern printf
extern scanf

section .data
inpMessage db 'Enter an Integer value:',0
outputMessage db `Output:%d\n`,0
true equ 1
false equ 0
intFormat db "%d",0
arrMessage db `Input: Enter %d array elements for range %d to %d\n`,0
section .text
main:
push rbp
mov rbp, rsp
sub rsp, 1440
jmp label1
label1:
mov rax, true
mov QWORD[rbp-8-192], rax
jmp label3
label2:
mov rax, false
mov QWORD[rbp-8-192], rax
jmp label3
label3:
mov rax, 5
mov QWORD[rbp-8-96], rax
mov rax, 9
mov QWORD[rbp-8-128], rax
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
lea rax, DWORD[rbp-8-32]
mov rsi, rax
call scanf WRT ..plt
movsxd rax, DWORD[rbp-8-32]
movsxd rbx, DWORD[rbp-8-128]
imul rax, rbx
mov QWORD[rbp-8-496], rax
movsxd rax, DWORD[rbp-8-96]
movsxd rbx, DWORD[rbp-8-128]
sub rax, rbx
mov DWORD[rbp-8-592], eax
movsxd rax, DWORD[rbp-8-592]
movsxd rbx, DWORD[rbp-8-32]
imul rax, rbx
mov QWORD[rbp-8-560], rax
movsxd rax, DWORD[rbp-8-96]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-656], rax
movsxd rax, DWORD[rbp-8-128]
movsxd rbx, DWORD[rbp-8-0]
imul rax, rbx
mov QWORD[rbp-8-688], rax
movsxd rax, DWORD[rbp-8-656]
movsxd rbx, DWORD[rbp-8-688]
sub rax, rbx
mov DWORD[rbp-8-624], eax
movsxd rax, DWORD[rbp-8-560]
movsxd rbx, DWORD[rbp-8-624]
add rax, rbx
mov QWORD[rbp-8-528], rax
movsxd rax, DWORD[rbp-8-496]
movsxd rbx, DWORD[rbp-8-528]
add rax, rbx
mov QWORD[rbp-8-464], rax
movsxd rax, DWORD[rbp-8-0]
movsxd rbx, DWORD[rbp-8-464]
add rax, rbx
mov QWORD[rbp-8-432], rax
movsxd rax, DWORD[rbp-8-432]
mov QWORD[rbp-8-64], rax
movsxd rax, DWORD[rbp-8-64];hello
mov rbx, 10
cmp rax, rbx
jg label4
jmp label7
label7:
movsxd rax, DWORD[rbp-8-96];hello
movsxd rbx, DWORD[rbp-8-128]
cmp rax, rbx
jle label8
jmp label5
label8:
movsxd rax, DWORD[rbp-8-0];hello
movsxd rbx, DWORD[rbp-8-32]
cmp rax, rbx
jl label9
jmp label5
label9:
label4:
mov rax, true
mov QWORD[rbp-8-208], rax
jmp label6
label5:
mov rax, false
mov QWORD[rbp-8-208], rax
jmp label6
label6:
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-64]
mov rax,0
call printf WRT ..plt
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-208]
mov rax,0
call printf WRT ..plt
mov rdi, arrMessage
mov rsi, 6
mov rdx, -3
mov rcx, 2
mov rax, 0
call printf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-240]
mov rsi, rax
call scanf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-272]
mov rsi, rax
call scanf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-304]
mov rsi, rax
call scanf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-336]
mov rsi, rax
call scanf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-368]
mov rsi, rax
call scanf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-400]
mov rsi, rax
call scanf WRT ..plt
mov rax, 0
mov rbx, -3
sub rax, rbx
mov DWORD[rbp-8-896], eax
movsxd rax, DWORD[rbp-8-928]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-928], rax
movsxd rax, DWORD[rbp-8-928]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-960], rax
movsxd rax, DWORD[rbp-8-960]
mov rbx, 14
add rax, rbx
mov QWORD[rbp-8-992], rax
movsxd rax, DWORD[rbp-8--16]
mov QWORD[rbp-8-1024], rax
movsxd rax, DWORD[rbp-8-1024]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-864], rax
mov rax, 1
mov rbx, -3
sub rax, rbx
mov DWORD[rbp-8-1120], eax
movsxd rax, DWORD[rbp-8-1152]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1152], rax
movsxd rax, DWORD[rbp-8-1152]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-1184], rax
movsxd rax, DWORD[rbp-8-1184]
mov rbx, 14
add rax, rbx
mov QWORD[rbp-8-1216], rax
movsxd rax, DWORD[rbp-8--16]
mov QWORD[rbp-8-1248], rax
movsxd rax, DWORD[rbp-8-1248]
mov rbx, 3
imul rax, rbx
mov QWORD[rbp-8-1088], rax
mov rax, 3
mov rbx, -3
sub rax, rbx
mov DWORD[rbp-8-1280], eax
movsxd rax, DWORD[rbp-8-1312]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1312], rax
movsxd rax, DWORD[rbp-8-1312]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-1344], rax
movsxd rax, DWORD[rbp-8-1344]
mov rbx, 14
add rax, rbx
mov QWORD[rbp-8-1376], rax
movsxd rax, DWORD[rbp-8--16]
mov QWORD[rbp-8-1408], rax
movsxd rax, DWORD[rbp-8-1088]
movsxd rbx, DWORD[rbp-8-1408]
add rax, rbx
mov QWORD[rbp-8-1056], rax
movsxd rax, DWORD[rbp-8-864]
movsxd rbx, DWORD[rbp-8-1056]
add rax, rbx
mov QWORD[rbp-8-832], rax
movsxd rax, DWORD[rbp-8-0]
movsxd rbx, DWORD[rbp-8-832]
add rax, rbx
mov QWORD[rbp-8-800], rax
movsxd rax, DWORD[rbp-8-96]
movsxd rbx, DWORD[rbp-8-800]
add rax, rbx
mov QWORD[rbp-8-768], rax
movsxd rax, DWORD[rbp-8-768]
mov QWORD[rbp-8-32], rax
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-32]
mov rax,0
call printf WRT ..plt
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-224]
mov rax,0
call printf WRT ..plt
leave
ret
