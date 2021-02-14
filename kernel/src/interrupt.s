.section .text,  "ax"
.extern a07_regs
.global _interrupt_handler
_interrupt_handler:
  addi sp, sp, -44
  sw gp, 40(sp)
  sw ra, 36(sp)
  sw t0, 32(sp)
  sw t1, 28(sp)
  sw t2, 24(sp)
  sw a0, 20(sp)
  sw a1, 16(sp)
  sw a2, 12(sp)
  sw a3, 8(sp)
  sw a4, 4(sp)
  sw a5, 0(sp)

  .option norelax
  la gp, __global_pointer$

  la t1, a07_regs
  sw a0, 0(t1)
  sw a1, 4(t1)
  sw a2, 8(t1)
  sw a3, 12(t1)
  sw a4, 16(t1)
  sw a5, 20(t1)
  call c_interrupt_handler

  lw gp, 40(sp)
  lw ra, 36(sp)
  lw t0, 32(sp)
  lw t1, 28(sp)
  lw t2, 24(sp)
  lw a0, 20(sp)
  lw a1, 16(sp)
  lw a2, 12(sp)
  lw a3, 8(sp)
  lw a4, 4(sp)
  lw a5, 0(sp)
  addi sp, sp, 44
  mret
