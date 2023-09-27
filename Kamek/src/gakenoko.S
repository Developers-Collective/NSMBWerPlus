.text
.align 4

#include <asm_setup.S>

.extern twenty_four_or_alt_f
.extern thirty_one_or_alt_f

.global move		//bl @ 0x80a02de4
move:
	lwz r9, 4(r30)
	lis r10, twenty_four_or_alt_f@h
	ori r10, r10, twenty_four_or_alt_f@l
	clrlslwi r9, r9, 31, 2
	lfsx f0, r10, r9
	blr


.global foolMove	//bl @ 0x80a02f80
foolMove:
	lwz r9, 4(r30)
	lis r10, twenty_four_or_alt_f@h
	ori r10, r10, twenty_four_or_alt_f@l
	clrlslwi r9, r9, 31, 2
	lfsx f0, r10, r9
	blr


.global fall		//bl @ 0x80a032c8
fall:
	lwz r9, 4(r30)
	lis r10, thirty_one_or_alt_f@h
	ori r10, r10, thirty_one_or_alt_f@l
	clrlslwi r9, r9, 31, 2
	add r5, r10, r9
	blr