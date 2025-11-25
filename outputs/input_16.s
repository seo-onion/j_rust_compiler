.data
print_fmt: .string "%ld \n"
fmt_str: .string "%s\n"
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $0, %rsp
 movq $42, %rax
mov %rax, %rdi
call identidad_i64
movq %rax, %rsi
leaq print_fmt(%rip), %rdi
movl $0, %eax
call printf@PLT
movq $5, %rdi
call malloc@PLT
movb $'h',0(%rax)
movb $'o',1(%rax)
movb $'l',2(%rax)
movb $'a',3(%rax)
movb $0,4(%rax)
movq $4,%rcx
mov %rcx, %rdi
mov %rax, %rsi
call identidad_str
leaq fmt_str(%rip), %rdi   
movq %rax, %rsi   
movl $0, %eax
call printf@PLT      
.end_main:
leave
ret
.globl identidad_str
identidad_str:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %rdi,-8(%rbp)
movq %rsi,-16(%rbp)
movq -16(%rbp), %rax
movq -8(%rbp), %rcx
 jmp .end_identidad_str
.end_identidad_str:
leave
ret
.globl identidad_i64
identidad_i64:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
movq %rdi,-8(%rbp)
movq -8(%rbp), %rax
movq 0(%rbp), %rcx
 jmp .end_identidad_i64
.end_identidad_i64:
leave
ret
.section .note.GNU-stack,"",@progbits
