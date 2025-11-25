.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
 movq $24, %rax
movq %rax, -8(%rbp)
movq -8(%rbp), %rax
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
