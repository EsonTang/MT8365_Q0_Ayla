/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arm_arch_svc.h>
#include <asm_macros.S>
#include <context.h>

	.globl	workaround_mmu_runtime_exceptions

#define ESR_EL3_A64_SMC0	0x5e000000

vector_base workaround_mmu_runtime_exceptions

	.macro	apply_workaround _is_sync_exception
	stp	x0, x1, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X0]
	mrs	x1, sctlr_el3
	/* Disable MMU */
	bic	x1, x1, #SCTLR_M_BIT
	msr	sctlr_el3, x1
	isb
	/* Enable MMU */
	orr	x1, x1, #SCTLR_M_BIT
	msr	sctlr_el3, x1
	/*
	 * Defer ISB to avoid synchronizing twice in case we hit
	 * the workaround SMC call which will implicitly synchronize
	 * because of the ERET instruction.
	 */

	/*
	 * Ensure SMC is coming from A64 state on #0
	 * with W0 = SMCCC_ARCH_WORKAROUND_1
	 *
	 * This sequence evaluates as:
	 *    (W0==SMCCC_ARCH_WORKAROUND_1) ? (ESR_EL3==SMC#0) : (NE)
	 * allowing use of a single branch operation
	 */
	.if \_is_sync_exception
		orr	w1, wzr, #SMCCC_ARCH_WORKAROUND_1
		cmp	w0, w1
		mrs	x0, esr_el3
		mov_imm	w1, ESR_EL3_A64_SMC0
		ccmp	w0, w1, #0, eq
		/* Static predictor will predict a fall through */
		bne	1f
		eret
1:
	.endif

	/*
	 * Synchronize now to enable the MMU.  This is required
	 * to ensure the load pair below reads the data stored earlier.
	 */
	isb
	ldp	x0, x1, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X0]
	.endm

	/* ---------------------------------------------------------------------
	 * Current EL with SP_EL0 : 0x0 - 0x200
	 * ---------------------------------------------------------------------
	 */
vector_entry workaround_mmu_sync_exception_sp_el0
	b	sync_exception_sp_el0
	check_vector_size workaround_mmu_sync_exception_sp_el0

vector_entry workaround_mmu_irq_sp_el0
	b	irq_sp_el0
	check_vector_size workaround_mmu_irq_sp_el0

vector_entry workaround_mmu_fiq_sp_el0
	b	fiq_sp_el0
	check_vector_size workaround_mmu_fiq_sp_el0

vector_entry workaround_mmu_serror_sp_el0
	b	serror_sp_el0
	check_vector_size workaround_mmu_serror_sp_el0

	/* ---------------------------------------------------------------------
	 * Current EL with SP_ELx: 0x200 - 0x400
	 * ---------------------------------------------------------------------
	 */
vector_entry workaround_mmu_sync_exception_sp_elx
	b	sync_exception_sp_elx
	check_vector_size workaround_mmu_sync_exception_sp_elx

vector_entry workaround_mmu_irq_sp_elx
	b	irq_sp_elx
	check_vector_size workaround_mmu_irq_sp_elx

vector_entry workaround_mmu_fiq_sp_elx
	b	fiq_sp_elx
	check_vector_size workaround_mmu_fiq_sp_elx

vector_entry workaround_mmu_serror_sp_elx
	b	serror_sp_elx
	check_vector_size workaround_mmu_serror_sp_elx

	/* ---------------------------------------------------------------------
	 * Lower EL using AArch64 : 0x400 - 0x600
	 * ---------------------------------------------------------------------
	 */
vector_entry workaround_mmu_sync_exception_aarch64
	apply_workaround _is_sync_exception=1
	b	sync_exception_aarch64
	check_vector_size workaround_mmu_sync_exception_aarch64

vector_entry workaround_mmu_irq_aarch64
	apply_workaround _is_sync_exception=0
	b	irq_aarch64
	check_vector_size workaround_mmu_irq_aarch64

vector_entry workaround_mmu_fiq_aarch64
	apply_workaround _is_sync_exception=0
	b	fiq_aarch64
	check_vector_size workaround_mmu_fiq_aarch64

vector_entry workaround_mmu_serror_aarch64
	apply_workaround _is_sync_exception=0
	b	serror_aarch64
	check_vector_size workaround_mmu_serror_aarch64

	/* ---------------------------------------------------------------------
	 * Lower EL using AArch32 : 0x600 - 0x800
	 * ---------------------------------------------------------------------
	 */
vector_entry workaround_mmu_sync_exception_aarch32
	apply_workaround _is_sync_exception=1
	b	sync_exception_aarch32
	check_vector_size workaround_mmu_sync_exception_aarch32

vector_entry workaround_mmu_irq_aarch32
	apply_workaround _is_sync_exception=0
	b	irq_aarch32
	check_vector_size workaround_mmu_irq_aarch32

vector_entry workaround_mmu_fiq_aarch32
	apply_workaround _is_sync_exception=0
	b	fiq_aarch32
	check_vector_size workaround_mmu_fiq_aarch32

vector_entry workaround_mmu_serror_aarch32
	apply_workaround _is_sync_exception=0
	b	serror_aarch32
	check_vector_size workaround_mmu_serror_aarch32
