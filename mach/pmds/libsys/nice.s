.define _nice
.sect .text
.sect .rom
.sect .data
.sect .bss
.sect .text
.extern _nice
.sect .text
_nice:
tst.b -40(sp)
link	a6,#-0
move.w 8(a6), d2
ext.l d2
move.l d2,-(sp)
jsr __Snice
add.l #4,sp
unlk a6
rts
__Snice:		trap #0
.data2	0x22
			bcc	1f
			jmp	cerror
1:
			clr.l	d0
			rts
