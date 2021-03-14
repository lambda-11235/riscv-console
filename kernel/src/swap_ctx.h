.macro swap_ctx old_sym, new_sym
	csrw mscratch, t0

	.option push
	.option norelax
	la t0, \old_sym
	.option pop

	sw ra, 0(t0)
	sw sp, 4(t0)
	sw gp, 8(t0)
	sw tp, 12(t0)
	sw t1, 20(t0)
	sw t2, 24(t0)
	sw s0, 28(t0)
	sw s1, 32(t0)
	sw a0, 36(t0)
	sw a1, 40(t0)
	sw a2, 44(t0)
	sw a3, 48(t0)
	sw a4, 52(t0)
	sw a5, 56(t0)

	csrr t1, mepc
	sw t1, 60(t0)

	csrr t1, mscratch
	sw t1, 16(t0)


	.option push
	.option norelax
	la t0, \new_sym
	.option pop

	lw t1, 60(t0)
	csrw mepc, t1

	lw ra, 0(t0)
	lw sp, 4(t0)
	lw gp, 8(t0)
	lw tp, 12(t0)
	lw t1, 20(t0)
	lw t2, 24(t0)
	lw s0, 28(t0)
	lw s1, 32(t0)
	lw a0, 36(t0)
	lw a1, 40(t0)
	lw a2, 44(t0)
	lw a3, 48(t0)
	lw a4, 52(t0)
	lw a5, 56(t0)
	lw t0, 16(t0)
.endm
