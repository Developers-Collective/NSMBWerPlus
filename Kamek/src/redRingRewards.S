#include <asm_setup.S>

.text
.align 4

.extern returnFromChangeRedRingArray
.global ChangeRedRingArray
ChangeRedRingArray:
	lis		r28, redRingRewards@h
	ori		r28, r28, redRingRewards@l
	b		returnFromChangeRedRingArray


.data
.align 4
redRingRewards:
	.long 0x00000009 #Fire Flower
	.long 0x00000015 #Propeller Mushroom
	.long 0x00000011 #Penguin Suit
	.long 0x0000000E #Ice Flower
	.long 0x00000019 #Mini Mushroom
	.long 0x00000001 #Power Star
	.long 0x00000007 #1-UP Mushroom