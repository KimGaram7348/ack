#
/*
 * Raspberry Pi support library for the ACK
 * © 2013 David Given
 * This file is redistributable under the terms of the 3-clause BSD license.
 * See the file 'Copying' in the root of the distribution for the full text.
 */

! Declare segments (the order is important).

.sect .text
.sect .rom
.sect .data
.sect .bss

.sect .text

! Reads a single byte.

.define __sys_rawread
__sys_rawread:
	xorb ah, ah
	int 0x16
	xorb ah, ah
	ret
	