.section .text,  "ax"


.include "./src/swap_ctx.h"

.extern c_mcause
.extern a05_regs

.extern c_interrupt_handler
.extern c_syscall_handler

.extern current_ctx
.extern interrupt_ctx

.global _interrupt_handler
_interrupt_handler:
  # Change contexts
  swap_ctx current_ctx, interrupt_ctx

  csrr t0, mcause
  la t1, c_mcause
  sw t0, 0(t1)

  li t1, 0xB
  beq t0, t1, .syscall
  call c_interrupt_handler
  j .end
.syscall:
  call c_syscall_handler

.end:
  swap_ctx interrupt_ctx, current_ctx
  mret
