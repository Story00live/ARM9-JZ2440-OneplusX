	.file	"key.c"
	.text
	.align	2
	.global	S3C2440_KeyInitialized
	.type	S3C2440_KeyInitialized, %function
S3C2440_KeyInitialized:
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
	bic	r3, r3, #51
	str	r3, [r2, #0]
	mov	r2, #1442840576
	add	r2, r2, #80
	mov	r3, #1442840576
	add	r3, r3, #80
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	mov	r2, #1442840576
	add	r2, r2, #96
	mov	r3, #1442840576
	add	r3, r3, #96
	ldr	r3, [r3, #0]
	bic	r3, r3, #192
	str	r3, [r2, #0]
	mov	r2, #1442840576
	add	r2, r2, #96
	mov	r3, #1442840576
	add	r3, r3, #96
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	ldmfd	sp, {fp, sp, pc}
	.size	S3C2440_KeyInitialized, .-S3C2440_KeyInitialized
	.ident	"GCC: (GNU) 3.4.5"
