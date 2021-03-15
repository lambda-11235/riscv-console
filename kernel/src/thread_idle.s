.section .text,  "ax"

.global thread_idle
thread_idle:
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

.loop:
  # Make thread_exit syscall, with exit_code as first argument
  li a0, 385
  ecall
  j .loop
