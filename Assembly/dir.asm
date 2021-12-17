section .data
directory: db "tmp", 0

global _main
section .text
 _main:
  call mkdir
  call exit
  ret

mkdir:
  mov rax, 0x2000088
  mov rdi, qword directory
  mov rsi, 0x1ED
  syscall

exit:
  mov rax, 0x2000001
  mov rdi, 0
  syscall

