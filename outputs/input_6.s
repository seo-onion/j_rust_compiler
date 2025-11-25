.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl doble
doble:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
movq %rdi,-8(%rbp)
movq -8(%rbp), %rax
pushq %rax
movq -8(%rbp), %rax
movq %rax, %rcx
 popq %rax
addq %rcx, %rax
 jmp .end_doble
.end_doble:
leave
ret
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $0, %rsp
 movq $21, %rax
mov %rax, %rdi
call doble
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
