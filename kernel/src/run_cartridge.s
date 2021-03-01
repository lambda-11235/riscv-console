.section .text,  "ax"

.extern program_running

.global run_cartridge
run_cartridge:
  addi sp, sp, -4
  sw ra, 0(sp)

  la t0, program_running
  li t1, 1
  sw t1, 0(t0)

  li t0, 0x4000001C
  lw t1, 0(t0)
  andi t1, t1, (~3)
  jalr t1

  .option norelax
  la gp, __global_pointer$

  la t0, program_running
  li t1, 0
  sw t1, 0(t0)

  lw ra, 0(sp)
  addi sp, sp, 4
  ret
