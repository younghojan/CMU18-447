    addi    $sp,    $sp,        -16     # allocate stack space
    sw      $16,    0($sp)              # save r16
    add     $16,    $4,         $0      # r16 for arg n
    sw      $17,    4($sp)              # save r17
    add     $17,    $0,         $0      # r17 for a, init to 0
    sw      $18,    8($sp)              # save r18
    addi    $18,    $0,         1       # r18 for b, init to 1
    sw      $31,    12($sp)             # save return address
    add     $2,     $17,        $18     # c = a + b
branch:
    slti    $3,     $16,        2       # use r3 as temp
    bne     $3,     $0,         done
    add     $2,     $17,        $18     # c = a + b
    add     $17,    $18,        $0      # a = b
    add     $18,    $2,         $0      # b = c
    addi    $16,    $16,        -1      # n = n - 1
    j       branch
done:
    lw      $31,    12($sp)             # restore r31
    lw      $18,    8($sp)              # restore r18
    lw      $17,    4($sp)              # restore r17
    lw      $16,    0($sp)              # restore r16
    addi    $sp,    $sp,        16      # restore stack pointer
    jr      $31                         # return to caller
