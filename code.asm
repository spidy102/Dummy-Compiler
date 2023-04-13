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
intFormat db "%d",0
arrayPrintFormat db "%d ",0
arrMessage db `Input: Enter %d array elements for range %d to %d\n`,0
section .text
main:
push rbp
mov rbp, rsp
sub rsp, 1984
jmp label1
label1:
mov rax, true
mov QWORD[rbp-8-224], rax
jmp label3
label2:
mov rax, false
mov QWORD[rbp-8-224], rax
jmp label3
label3:
mov rax, 5
mov QWORD[rbp-8-128], rax
mov rax, 9
mov QWORD[rbp-8-160], rax
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
movsxd rbx, DWORD[rbp-8-160]
imul rax, rbx
mov QWORD[rbp-8-624], rax
movsxd rax, DWORD[rbp-8-128]
movsxd rbx, DWORD[rbp-8-160]
sub rax, rbx
mov QWORD[rbp-8-720], rax
movsxd rax, DWORD[rbp-8-720]
movsxd rbx, DWORD[rbp-8-32]
imul rax, rbx
mov QWORD[rbp-8-688], rax
movsxd rax, DWORD[rbp-8-128]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-784], rax
movsxd rax, DWORD[rbp-8-160]
movsxd rbx, DWORD[rbp-8-0]
imul rax, rbx
mov QWORD[rbp-8-816], rax
movsxd rax, DWORD[rbp-8-784]
movsxd rbx, DWORD[rbp-8-816]
sub rax, rbx
mov QWORD[rbp-8-752], rax
movsxd rax, DWORD[rbp-8-688]
movsxd rbx, DWORD[rbp-8-752]
add rax, rbx
mov QWORD[rbp-8-656], rax
movsxd rax, DWORD[rbp-8-624]
movsxd rbx, DWORD[rbp-8-656]
add rax, rbx
mov QWORD[rbp-8-592], rax
movsxd rax, DWORD[rbp-8-0]
movsxd rbx, DWORD[rbp-8-592]
add rax, rbx
mov QWORD[rbp-8-560], rax
movsxd rax, DWORD[rbp-8-560]
mov QWORD[rbp-8-64], rax
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-64]
mov rax,0
call printf WRT ..plt
mov rax, 10
mov QWORD[rbp-8-464], rax
label4:
movsxd rax, DWORD[rbp-8-464];hello
mov rbx, 15
cmp rax, rbx
jle label5
jmp label6
label5:
mov rax, 7
mov QWORD[rbp-8-496], rax
mov rdi, inpMessage
mov rax, 0
call printf WRT ..plt
mov rdi, intFormat
lea rax, DWORD[rbp-8-528]
mov rsi, rax
call scanf WRT ..plt
movsxd rax, DWORD[rbp-8-496]
movsxd rbx, DWORD[rbp-8-528]
add rax, rbx
mov QWORD[rbp-8-848], rax
movsxd rax, DWORD[rbp-8-848]
mov QWORD[rbp-8-496], rax
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-496]
mov rax,0
call printf WRT ..plt
movsxd rax, DWORD[rbp-8-496]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-944], rax
movsxd rax, DWORD[rbp-8-464]
movsxd rbx, DWORD[rbp-8-944]
add rax, rbx
mov QWORD[rbp-8-912], rax
movsxd rax, DWORD[rbp-8-64]
movsxd rbx, DWORD[rbp-8-912]
add rax, rbx
mov QWORD[rbp-8-880], rax
movsxd rax, DWORD[rbp-8-880]
mov QWORD[rbp-8-64], rax
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-64]
mov rax,0
call printf WRT ..plt
movsxd rax, DWORD[rbp-8-464]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-464], rax
jmp label4
label6:
movsxd rax, DWORD[rbp-8-32]
movsxd rbx, DWORD[rbp-8-160]
imul rax, rbx
mov QWORD[rbp-8-1040], rax
movsxd rax, DWORD[rbp-8-128]
movsxd rbx, DWORD[rbp-8-160]
sub rax, rbx
mov QWORD[rbp-8-1136], rax
movsxd rax, DWORD[rbp-8-1136]
movsxd rbx, DWORD[rbp-8-32]
imul rax, rbx
mov QWORD[rbp-8-1104], rax
movsxd rax, DWORD[rbp-8-128]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1200], rax
movsxd rax, DWORD[rbp-8-160]
movsxd rbx, DWORD[rbp-8-0]
imul rax, rbx
mov QWORD[rbp-8-1232], rax
movsxd rax, DWORD[rbp-8-1200]
movsxd rbx, DWORD[rbp-8-1232]
sub rax, rbx
mov QWORD[rbp-8-1168], rax
movsxd rax, DWORD[rbp-8-1104]
movsxd rbx, DWORD[rbp-8-1168]
add rax, rbx
mov QWORD[rbp-8-1072], rax
movsxd rax, DWORD[rbp-8-1040]
movsxd rbx, DWORD[rbp-8-1072]
add rax, rbx
mov QWORD[rbp-8-1008], rax
movsxd rax, DWORD[rbp-8-0]
movsxd rbx, DWORD[rbp-8-1008]
add rax, rbx
mov QWORD[rbp-8-976], rax
movsxd rax, DWORD[rbp-8-976]
mov QWORD[rbp-8-64], rax
movsxd rax, DWORD[rbp-8-64];hello
mov rbx, 10
cmp rax, rbx
jg label7
jmp label10
label10:
movsxd rax, DWORD[rbp-8-128];hello
movsxd rbx, DWORD[rbp-8-160]
cmp rax, rbx
jle label11
jmp label8
label11:
movsxd rax, DWORD[rbp-8-0];hello
movsxd rbx, DWORD[rbp-8-32]
cmp rax, rbx
jl label12
jmp label8
label12:
label7:
mov rax, true
mov QWORD[rbp-8-240], rax
jmp label9
label8:
mov rax, false
mov QWORD[rbp-8-240], rax
jmp label9
label9:
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-64]
mov rax,0
call printf WRT ..plt
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-224]
mov rax,0
call printf WRT ..plt
mov rdi, arrMessage
mov rsi, 6
mov rdx, 10
mov rcx, 15
mov rax, 0
call printf WRT ..plt
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
mov rdi, intFormat
lea rax, DWORD[rbp-8-432]
mov rsi, rax
call scanf WRT ..plt
mov rax, 13
mov rbx, 10
sub rax, rbx
mov QWORD[rbp-8-1440], rax
movsxd rax, DWORD[rbp-8-1440]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1472], rax
movsxd rax, DWORD[rbp-8-1472]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-1504], rax
movsxd rax, DWORD[rbp-8-1504]
mov rbx, 16
add rax, rbx
mov QWORD[rbp-8-1536], rax
movsxd rax, DWORD[rbp-8-1536]
mov rbx, rbp
imul rax, 16
sub rbx, rax
sub rbx,8
movsxd rax, DWORD[rbx]
mov QWORD[rbp-8-1568], rax
movsxd rax, DWORD[rbp-8-1568]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1408], rax
mov rax, 14
mov rbx, 10
sub rax, rbx
mov QWORD[rbp-8-1664], rax
movsxd rax, DWORD[rbp-8-1664]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1696], rax
movsxd rax, DWORD[rbp-8-1696]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-1728], rax
movsxd rax, DWORD[rbp-8-1728]
mov rbx, 16
add rax, rbx
mov QWORD[rbp-8-1760], rax
movsxd rax, DWORD[rbp-8-1760]
mov rbx, rbp
imul rax, 16
sub rbx, rax
sub rbx,8
movsxd rax, DWORD[rbx]
mov QWORD[rbp-8-1792], rax
movsxd rax, DWORD[rbp-8-1792]
mov rbx, 3
imul rax, rbx
mov QWORD[rbp-8-1632], rax
mov rax, 15
mov rbx, 10
sub rax, rbx
mov QWORD[rbp-8-1824], rax
movsxd rax, DWORD[rbp-8-1824]
mov rbx, 2
imul rax, rbx
mov QWORD[rbp-8-1856], rax
movsxd rax, DWORD[rbp-8-1856]
mov rbx, 1
add rax, rbx
mov QWORD[rbp-8-1888], rax
movsxd rax, DWORD[rbp-8-1888]
mov rbx, 16
add rax, rbx
mov QWORD[rbp-8-1920], rax
movsxd rax, DWORD[rbp-8-1920]
mov rbx, rbp
imul rax, 16
sub rbx, rax
sub rbx,8
movsxd rax, DWORD[rbx]
mov QWORD[rbp-8-1952], rax
movsxd rax, DWORD[rbp-8-1632]
movsxd rbx, DWORD[rbp-8-1952]
add rax, rbx
mov QWORD[rbp-8-1600], rax
movsxd rax, DWORD[rbp-8-1408]
movsxd rbx, DWORD[rbp-8-1600]
add rax, rbx
mov QWORD[rbp-8-1376], rax
movsxd rax, DWORD[rbp-8-0]
movsxd rbx, DWORD[rbp-8-1376]
add rax, rbx
mov QWORD[rbp-8-1344], rax
movsxd rax, DWORD[rbp-8-128]
movsxd rbx, DWORD[rbp-8-1344]
add rax, rbx
mov QWORD[rbp-8-1312], rax
movsxd rax, DWORD[rbp-8-1312]
mov QWORD[rbp-8-32], rax
mov rdi, outputMessage
movsxd rsi, DWORD[rbp-8-32]
mov rax,0
call printf WRT ..plt
mov rdi, outputMessageArray
mov rax,0
call printf WRT ..plt
mov rdi, arrayPrintFormat
movsxd rsi, DWORD[rbp-8-272]
call printf WRT ..plt
mov rdi, arrayPrintFormat
movsxd rsi, DWORD[rbp-8-304]
call printf WRT ..plt
mov rdi, arrayPrintFormat
movsxd rsi, DWORD[rbp-8-336]
call printf WRT ..plt
mov rdi, arrayPrintFormat
movsxd rsi, DWORD[rbp-8-368]
call printf WRT ..plt
mov rdi, arrayPrintFormat
movsxd rsi, DWORD[rbp-8-400]
call printf WRT ..plt
mov rdi, arrayPrintFormat
movsxd rsi, DWORD[rbp-8-432]
call printf WRT ..plt
mov rdi, newLine
call printf WRT ..plt
leave
ret
