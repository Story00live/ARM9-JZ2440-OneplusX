	.file	"main.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #8
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	bl	S3C2440_LedInitialized
	bl	S3C2440_KeyInitialized
.L2:
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L4
	mov	r2, #1442840576
	add	r2, r2, #84
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	orr	r3, r3, #16
	str	r3, [r2, #0]
	b	.L5
.L4:
	mov	r2, #1442840576
	add	r2, r2, #84
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	bic	r3, r3, #16
	str	r3, [r2, #0]
.L5:
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	and	r3, r3, #4
	cmp	r3, #0
	beq	.L6
	mov	r2, #1442840576
	add	r2, r2, #84
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	orr	r3, r3, #32
	str	r3, [r2, #0]
	b	.L7
.L6:
	mov	r2, #1442840576
	add	r2, r2, #84
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	bic	r3, r3, #32
	str	r3, [r2, #0]
.L7:
	mov	r3, #1442840576
	add	r3, r3, #100
	ldr	r3, [r3, #0]
	and	r3, r3, #8
	cmp	r3, #0
	beq	.L8
	mov	r2, #1442840576
	add	r2, r2, #84
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	orr	r3, r3, #64
	str	r3, [r2, #0]
	b	.L2
.L8:
	mov	r2, #1442840576
	add	r2, r2, #84
	mov	r3, #1442840576
	add	r3, r3, #84
	ldr	r3, [r3, #0]
	bic	r3, r3, #64
	str	r3, [r2, #0]
	b	.L2
	.size	main, .-main
	.ident	"GCC: (GNU) 3.4.5"
