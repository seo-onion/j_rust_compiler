.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $0, %rsp
 movq $10, %rax
mov %rax, %rdi
 movq $20, %rax
mov %rax, %rsi
call primero_i64_i64
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
 movq $30, %rax
mov %rax, %rdi
 movq $40, %rax
mov %rax, %rsi
call primero_i64_i64
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
 movq $30, %rax
mov %rax, %rdi
 movq $40, %rax
mov %rax, %rsi
call segundo_i64_i64
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
.end_main:
leave
ret
.globl segundo_i64_i64
segundo_i64_i64:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %rdi,-8(%rbp)
movq %rsi,-16(%rbp)
movq -16(%rbp), %rax
movq -8(%rbp), %rcx
 jmp .end_segundo_i64_i64
.end_segundo_i64_i64:
leave
ret
.globl primero_i64_i64
primero_i64_i64:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %rdi,-8(%rbp)
movq %rsi,-16(%rbp)
movq -8(%rbp), %rax
movq 0(%rbp), %rcx
 jmp .end_primero_i64_i64
.end_primero_i64_i64:
leave
ret
.section .note.GNU-stack,"",@progbits
