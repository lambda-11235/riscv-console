.section .init, "ax"
.global _start
_start:
    .option push
    .option norelax
    addi sp, sp, -4
    sw ra, 0(sp)

    la gp, __global_pointer$
    call init
    nop
    call main
    nop

    lw ra, 0(sp)
    addi sp, sp, 4
    ret
