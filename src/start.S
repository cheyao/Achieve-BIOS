.section .text
.globl _start
_start: 
  li x2, 0x100000
  call main
  jr a0 
