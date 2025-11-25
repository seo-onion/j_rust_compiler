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
 movq $1, %rax
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
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $0, %rsp
 movq $10, %rax
mov %rax, %rdi
call fib
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
