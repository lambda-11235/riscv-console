.section .text,  "ax"

.extern fault
.extern thread_create

.extern program_running

.global run_cartridge
run_cartridge:
  addi sp, sp, -4
  sw ra, 0(sp)

  la t0, program_running
  li t1, 1
  sw t1, 0(t0)

  # Create a new thread for the user process
  li t0, 0x4000001C
  lw a0, 0(t0)
  andi a0, a0, (~3)
  call thread_create

  li t0, -1
  bne a0, t0, .skip_fault1
  la a0, create_error
  call fault
.skip_fault1:

  # Syscall to thread_join causes us to switch and wait for process to finish.
  mv a1, a0
  la a2, exit_status
  li a0, 386
  ecall

  li t0, -1
  bne a0, t0, .skip_fault2
  la a0, join_error
  call fault
.skip_fault2:

  # Return exit status
  la t0, exit_status
  lw a0, 0(t0)

  la t0, program_running
  li t1, 0
  sw t1, 0(t0)

  lw ra, 0(sp)
  addi sp, sp, 4
  ret


.section .data

exit_status: .word 0
create_error: .ascii "Couldn't create thread for main process"
join_error: .ascii "Couldn't join thread for main process"
