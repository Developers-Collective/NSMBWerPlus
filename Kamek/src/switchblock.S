.text

#ifndef __MWERKS__
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31; .set f0,0; .set f2,2; .set f3,3
#endif

.align 4
.extern GameMgrP
.extern BG_GM_ptr
.extern _restgpr_27

.global BG_GM_InitRedSwitchFlag_Patch
BG_GM_InitRedSwitchFlag_Patch:
	lis r5, GameMgrP@h
	ori r5, r5, GameMgrP@l
	lwz r5, 0(r5)

	addis r4, r3, 9
	lbz r0, 0x380(r5)
	stb r0, 0xAA8(r4)
	blr

.global CheckBehaviourOverride_EndPatch
CheckBehaviourOverride_EndPatch:
	lis r5, BG_GM_ptr@h
	ori r5, r5, BG_GM_ptr@l
	lwz r5, 0(r5)

	# Get the switch value
	addis r5, r5, 9
	lbz r5, 0xAA8(r5)

	# not sure why this happens
	cmpwi r28, 0
	bne returnRegularBehaviour

	# now do the rest of the checks:
	cmplwi r29, 0x58
	bne cboNotRed
	andi. r3, r5, 1
	beq cboNotRed
	b returnSbBehaviour
cboNotRed:
	cmplwi r29, 0x5A
	bne cboNotGreen
	andi. r3, r5, 2
	beq cboNotGreen
	b returnSbBehaviour
cboNotGreen:
	cmplwi r29, 0x56
	bne cboNotYellow
	andi. r3, r5, 4
	beq cboNotYellow
	b returnSbBehaviour
cboNotYellow:
	cmplwi r29, 0x54
	bne cboNotBlue
	andi. r3, r5, 8
	beq cboNotBlue
	b returnSbBehaviour
cboNotBlue:
	cmplwi r29, 0x3B
	bne cboNotWM1
	andi. r3, r5, 0x10
	beq cboNotWM1
	b returnSbBehaviour
cboNotWM1:
	cmplwi r29, 0x3C
	bne cboNotWM2
	andi. r3, r5, 0x10
	beq cboNotWM2
	b returnSbBehaviour
cboNotWM2:
	b returnRegularBehaviour

	# done!
returnSbBehaviour:
	li r4, 3
	li r3, 8
	b cboReturn
returnRegularBehaviour:
	mr r4, r27
	mr r3, r28
cboReturn:
	addi r11, r1, 0x20
	bl _restgpr_27
	lwz r0, 0x24(r1)
	mtlr r0
	addi r1, r1, 0x20
	blr

.global ProcessTileNum_EndPatch
ProcessTileNum_EndPatch:
	lis r5, BG_GM_ptr@h
	ori r5, r5, BG_GM_ptr@l
	lwz r5, 0(r5)
	addis r5, r5, 9
	lbz r5, 0xAA8(r5)

	cmplwi r30, 0x58
	bne not_red
	andi. r3, r5, 1
	beq not_red
	li r3, 0x57
	b ptnReturn
not_red:
	cmplwi r30, 0x5A
	bne not_green
	andi. r3, r5, 2
	beq not_green
	li r3, 0x59
	b ptnReturn
not_green:
	cmplwi r30, 0x56
	bne not_yellow
	andi. r3, r5, 4
	beq not_yellow
	li r3, 0x55
	b ptnReturn
not_yellow:
	cmplwi r30, 0x54
	bne not_blue
	andi. r3, r5, 8
	beq not_blue
	li r3, 0x53
	b ptnReturn
not_blue:
	cmplwi r30, 0x3B
	bne not_WM1
	andi. r3, r5, 0x10
	beq not_WM1
	li r3, 0x37
	b ptnReturn
not_WM1:
	cmplwi r30, 0x3C
	bne not_WM2
	andi. r3, r5, 0x10
	beq not_WM2
	li r3, 0x37
	b ptnReturn
not_WM2:
	mr r3, r30
ptnReturn:
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	lwz r30, 0x8(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr


.global checkSwitchBit5_r0r3
checkSwitchBit5_r0r3:
	lbz r0, 0x380(r3)
	srwi r0, r0, 4
	blr

.global checkSwitchBit5_r0r4
checkSwitchBit5_r0r4:
	lbz r0, 0x380(r4)
	srwi r0, r0, 4
	blr

.global checkSwitchBit5_r4r4
checkSwitchBit5_r4r4:
	lbz r4, 0x380(r4)
	srwi r4, r4, 4
	blr

.global checkSwitchBit5_r0r5
checkSwitchBit5_r0r5:
	lbz r0, 0x380(r5)
	srwi r0, r0, 4
	blr

.extern continueReplayChooser
.global checkSwitchBit5_ReplayChooser
checkSwitchBit5_ReplayChooser:
	lbz r0, 0x380(r6)
	srwi r0, r0, 4
	b continueReplayChooser

.global checkSwitchBit5_r6r6
checkSwitchBit5_r6r6:
	lbz r6, 0x380(r6)
	srwi r6, r6, 4
	blr