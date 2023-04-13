global main
extern printf
extern scanf

section .data
inpMessage db 'Enter an Integer value:',0
outputMessage db `Output:%d\n`,0
outputMessageArray db `Output:`,0
newLine db `\n`,0
true equ 1
false equ 0
trueFormat db "true",0
falseFormat db "false",0
intFormat db "%d",0
arrayPrintFormat db "%d ",0
arrMessage db `Input: Enter %d array elements for range %d to %d\n`,0
section .text
main:
push rbp
mov rbp, rsp
sub rsp, 560
jmp label2
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
mov QWORD[rbp-8-288], rax
movsxd rax, DWORD[rbp-8-96]
movsxd rbx, DWORD[rbp-8-128]
sub rax, rbx
mov QWORD[rbp-8-384], rax
movsxd rax, DWORD[rbp-8-384]
movsxd rbx, DWORD[rbp-8-32]
imul rax, rbx
mov QWORD[rbp-8-352], rax
movsxd rax, DWORD[rbp-8-96]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-448], rax
movsxd rax, DWORD[rbp-8-128]
movsxd rbx, DWORD[rbp-8-0]
imul rax, rbx
mov QWORD[rbp-8-480], rax
movsxd rax, DWORD[rbp-8-448]
movsxd rbx, DWORD[rbp-8-480]
sub rax, rbx
mov QWORD[rbp-8-416], rax
movsxd rax, DWORD[rbp-8-352]
movsxd rbx, DWORD[rbp-8-416]
add rax, rbx
mov QWORD[rbp-8-320], rax
movsxd rax, DWORD[rbp-8-288]
movsxd rbx, DWORD[rbp-8-320]
add rax, rbx
mov QWORD[rbp-8-256], rax
movsxd rax, DWORD[rbp-8-256]
mov rbx, 0
add rax, rbx
mov QWORD[rbp-8-224], rax
movsxd rax, DWORD[rbp-8-224]
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
mov rdi, outputMessageArray
mov rax,0
call printf WRT ..plt
movsxd rsi, DWORD[rbp-8-208]
cmp rsi,1
je label10
jne label11
label10:
mov rdi, trueFormat
mov rax,0
call printf WRT ..plt
jmp label12
label11:
mov rdi, falseFormat
mov rax,0
call printf WRT ..plt
jmp label12
label12:
mov rdi, newLine
mov rax,0
call printf WRT ..plt
leave
ret
