.sect .text; .sect .rom; .sect .data; .sect .bss
.sect .text
write = 4
.define	__write

__write:
	.data2	0x0000
	chmk	$write
	bcc 	1f
	jmp 	cerror
1:
	ret
