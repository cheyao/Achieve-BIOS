.section .text
.globl _start

_start:
        li      a5,-1             # tmp74,
        slli    a5,a5,63        #, tmp75, tmp74
        li      a4,50             # tmp72,
        addi    a3,a5,1 #, tmp73, tmp75
        sh a4, 0(a3)    # tmp72, tmp73
        addi    a3,a5,2 #, tmp77, tmp75
        sh a4, 0(a3)    # tmp72, tmp77
        li      a4,-1             # tmp80,
        sh a4, 0(a5)    # tmp80, tmp75
        ebreak     
