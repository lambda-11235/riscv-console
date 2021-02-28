.section .text,  "ax"

.extern c_save_user_gp
        
.global signal_raise_user
signal_raise_user:
  addi sp, sp, -20
  sw ra, 0(sp)

  # Save interrupt information
  csrr t0, mip
  sw t0, 4(sp)
  csrr t0, mie
  sw t0, 8(sp)
  csrr t0, mepc
  sw t0, 12(sp)
  csrr t0, mstatus
  sw t0, 16(sp)


  la t0, c_save_user_gp
  lw gp, 0(t0)

  # Enable software interrupts
  csrsi mie, 8

  jalr a1

  .option norelax
  la gp, __global_pointer$


  lw ra, 0(sp)

  # Restore interrupts
  lw t0, 4(sp)
  csrw mip, t0
  lw t0, 8(sp)
  csrw mie, t0
  lw t0, 12(sp)
  csrw mepc, t0
  lw t0, 16(sp)
  csrw mstatus, t0

  addi sp, sp, 20

  ret
