.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl suma
suma:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %rdi,-8(%rbp)
 movq $10, %rax
movq %rax, -16(%rbp)
movq -8(%rbp), %rax
pushq %rax
movq -16(%rbp), %rax
movq %rax, %rcx
 popq %rax
addq %rcx, %rax
 jmp .end_suma
.end_suma:
leave
ret
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $0, %rsp
 movq $5, %rax
mov %rax, %rdi
call suma
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
