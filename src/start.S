.section .text
.globl _start
_start:
  li x2, 0x1000
  call main
  ebreak
