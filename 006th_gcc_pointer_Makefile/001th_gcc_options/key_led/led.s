	.file	"led.c"
	.text
	.align	2
	.global	delay
	.type	delay, %function
delay:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #4
	str	r0, [fp, #-16]
.L2:
	ldr	r3, [fp, #-16]
	sub	r3, r3, #1
	str	r3, [fp, #-16]
	cmn	r3, #1
	beq	.L1
	b	.L2
.L1:
	ldmfd	sp, {r3, fp, sp, pc}
	.size	delay, .-delay
	.align	2
	.global	led_on
	.type	led_on, %function
led_on:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #4
	str	r0, [fp, #-16]
	mov	r3, #1442840576
	add	r3, r3, #80
	mov	r2, #5376
	str	r2, [r3, #0]
	ldr	r3, [fp, #-16]
	cmp	r3, #1
	bne	.L5
	mov	r3, #1442840576
	add	r3, r3, #84
	mov	r2, #0
	str	r2, [r3, #0]
	b	.L6
.L5:
	mov	r3, #1442840576
	add	r3, r3, #84
	mvn	r2, #0
	str	r2, [r3, #0]
.L6:
	mov	r0, r3
	ldmfd	sp, {r3, fp, sp, pc}
	.size	led_on, .-led_on
	.align	2
	.global	S3C2440_LedInitialized
	.type	S3C2440_LedInitialized, %function
S3C2440_LedInitialized:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	mov	r2, #1442840576
	add	r2, r2, #80
	mov	r3, #1442840576
	add	r3, r3, #80
	ldr	r3, [r3, #0]
	bic	r3, r3, #16128
	str	r3, [r2, #0]
	mov	r2, #1442840576
	add	r2, r2, #80
	mov	r3, #1442840576
	add	r3, r3, #80
	ldr	r3, [r3, #0]
	orr	r3, r3, #5376
	str	r3, [r2, #0]
	ldmfd	sp, {fp, sp, pc}
	.size	S3C2440_LedInitialized, .-S3C2440_LedInitialized
	.ident	"GCC: (GNU) 3.4.5"
