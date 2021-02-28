.section .text

.global syscall
syscall:
  addi sp, sp, -4
  sw ra, 0(sp)

  mv t1, a1
  lw a1, 0(t1)
  lw a2, 4(t1)
  lw a3, 8(t1)
  lw a4, 12(t1)
  lw a5, 16(t1)
  ecall

  lw ra, 0(sp)
  addi sp, sp, 4
  ret
