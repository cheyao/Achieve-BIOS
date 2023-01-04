_start:
        li      a5,209727488                # tmp75,
        li      a4,-1             # tmp73,
        slli    a5,a5,5 #, tmp76, tmp75
        slli    a4,a4,63        #, tmp72, tmp73
        addi    a5,a5,-1        #, tmp74, tmp76
        sd      a5,0(a4)    # tmp74, MEM[(volatile uint64_t *)-9223372036854775808B]
        ret     
