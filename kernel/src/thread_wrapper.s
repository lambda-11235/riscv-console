.section .text,  "ax"

.global thread_wrapper
thread_wrapper:
  # Call thread main function
  mv t0, a0
  mv a0, a1
  jalr t0

  # Make thread_exit syscall, with exit_code as first argument
  mv a1, a0
  li a0, 387
  ecall
