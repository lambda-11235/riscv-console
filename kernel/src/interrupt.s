.section .text,  "ax"

.extern c_mcause
.extern c_mip
.extern a05_regs

.extern c_interrupt_handler
.extern c_syscall_handler

.global _interrupt_handler
_interrupt_handler:
  # Always save global pointer, because C functions may rely on it.
  # Also save temps because we need them to retreive interrupt info.
  addi sp, sp, -20
  sw ra, 16(sp)
  sw t0, 12(sp)
  sw t1, 8(sp)
  sw t2, 4(sp)
  sw gp, 0(sp)

  .option norelax
  la gp, __global_pointer$

  csrr t0, mip
  la t1, c_mip
  lw t0, 0(t1)

  csrr t0, mcause
  la t1, c_mcause
  sw t0, 0(t1)

  li t1, 0xB
  beq t0, t1, .syscall
  j _async
.syscall:
  j _syscall
.end:

  lw ra, 16(sp)
  lw t0, 12(sp)
  lw t1, 8(sp)
  lw t2, 4(sp)
  lw gp, 0(sp)
  add sp, sp, 20

  mret

_async:
  addi sp, sp, -24
  sw a0, 20(sp)
  sw a1, 16(sp)
  sw a2, 12(sp)
  sw a3, 8(sp)
  sw a4, 4(sp)
  sw a5, 0(sp)

  call c_interrupt_handler

  lw a0, 20(sp)
  lw a1, 16(sp)
  lw a2, 12(sp)
  lw a3, 8(sp)
  lw a4, 4(sp)
  lw a5, 0(sp)
  addi sp, sp, 24
  j .end

_syscall:
  # No need to save everything, as ecall follows the C calling convention
  la t0, a05_regs
  sw a0, 0(t0)
  sw a1, 4(t0)
  sw a2, 8(t0)
  sw a3, 12(t0)
  sw a4, 16(t0)
  sw a5, 20(t0)

  call c_syscall_handler

  # Return interrupt to right address
  csrr t0, mepc
  addi t0, t0, 4
  csrw mepc, t0

  j .end
