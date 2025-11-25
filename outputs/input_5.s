.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl xd
xd:
pushq %rbp
movq %rsp, %rbp
subq $32, %rsp
# Se compila directamente el cuerpo del while porque la consicion siempre es verdadera
while_0:
 movq $3, %rax
movq %rax, -8(%rbp)
 movq $4, %rax
movq %rax, -16(%rbp)
movq -8(%rbp), %rax
pushq %rax
movq -16(%rbp), %rax
movq %rax, %rcx
 popq %rax
cmpq %rcx, %rax
movl $0, %eax
setl %al
movzbq %al, %rax
 cmpq $0, %rax
 je else_1
movq -8(%rbp), %rax
pushq %rax
movq -16(%rbp), %rax
movq %rax, %rcx
 popq %rax
addq %rcx, %rax
movq %rax, -32(%rbp)
movq -32(%rbp), %rax
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
 jmp endif_1
 else_1:
movq -8(%rbp), %rax
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
endif_1:
 movq $0, %rax
 jmp .end_xd
while_0:
.end_xd:
leave
ret
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $0, %rsp
call xd
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
