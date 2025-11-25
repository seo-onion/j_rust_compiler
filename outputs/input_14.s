.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl fib
fib:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
movq %rdi,-8(%rbp)
movq -8(%rbp), %rax
pushq %rax
 movq $2, %rax
movq %rax, %rcx
 popq %rax
cmpq %rcx, %rax
movl $0, %eax
setl %al
movzbq %al, %rax
 cmpq $0, %rax
 je else_0
movq -8(%rbp), %rax
 jmp .end_fib
 jmp endif_0
 else_0:
movq -8(%rbp), %rax
pushq %rax
 movq $1, %rax
movq %rax, %rcx
 popq %rax
subq %rcx, %rax
mov %rax, %rdi
call fib
pushq %rax
movq -8(%rbp), %rax
pushq %rax
 movq $2, %rax
movq %rax, %rcx
 popq %rax
subq %rcx, %rax
mov %rax, %rdi
call fib
movq %rax, %rcx
 popq %rax
addq %rcx, %rax
 jmp .end_fib
endif_0:
.end_fib:
leave
ret
.globl fact
fact:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
movq %rdi,-8(%rbp)
movq -8(%rbp), %rax
pushq %rax
 movq $2, %rax
movq %rax, %rcx
 popq %rax
cmpq %rcx, %rax
movl $0, %eax
setl %al
movzbq %al, %rax
 cmpq $0, %rax
 je else_1
 movq $1, %rax
 jmp .end_fact
 jmp endif_1
 else_1:
endif_1:
movq -8(%rbp), %rax
pushq %rax
movq -8(%rbp), %rax
pushq %rax
 movq $1, %rax
movq %rax, %rcx
 popq %rax
subq %rcx, %rax
mov %rax, %rdi
call fact
movq %rax, %rcx
 popq %rax
imulq %rcx, %rax
 jmp .end_fact
.end_fact:
leave
ret
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
 movq $1, %rax
movq %rax, -8(%rbp)
 movq $15, %rax
mov %rax, %rdi
call fact
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
