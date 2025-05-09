.text
.extern StaffCreditsAddCoin
.extern CreditsModeActive
.extern continueAddToCoinCount
.global StaffCreditsAddAllCoins
StaffCreditsAddAllCoins:
	lis r4, CreditsModeActive@h
	ori r4, r4, CreditsModeActive@l
	lbz r4, 0(r4)
	cmpwi r4, 1
	beq isCredits
	stwu r1, -0x40(r1)
	b continueAddToCoinCount
isCredits:
	b StaffCreditsAddCoin


.extern continueAddToScore
.global StaffCreditsNoScore
StaffCreditsNoScore:
	lis r9, isNewerCredits@h
	ori r9, r9, isNewerCredits@l
	lbz r9, 0(r9)
	cmpwi r9, 1
	beqlr
	cmpwi r5, -1
	b continueAddToScore


.global AutoScrollInjector
.extern BG_GM_ptr
AutoScrollInjector:
	lis r3, CameraLockEnabled@h
	ori r3, r3, CameraLockEnabled@l
	lbz r3, 0(r3)
	cmpwi r3, 1
	bnelr

	lis r3, BG_GM_ptr@h
	ori r3, r3, BG_GM_ptr@l
	lwz r3, 0(r3)

	lis r4, CameraLockPosition@h
	ori r4, r4, CameraLockPosition@l

	addis r3, r3, 9
	lfs f1, 0(r4)
	stfs f1, -0x178(r3)
	lfs f1, 4(r4)
	stfs f1, -0x174(r3)
	blr


.global PlayerRotInjector
PlayerRotInjector:
	lis r9, isLockPlayerRotation@h
	ori r9, r9, isLockPlayerRotation@l
	lbz r9, 0(r9)
	cmpwi r9, 1
	bne dontOverridePlayerRot
	lis r9, lockedPlayerRotation@h
	ori r9, r9, lockedPlayerRotation@l
	lha r7, 0(r9)
dontOverridePlayerRot:
	sth r7, 0xA(r1)
	blr

.global PlayerRotInjector2
PlayerRotInjector2:
	lis r9, isLockPlayerRotation@h
	ori r9, r9, isLockPlayerRotation@l
	lbz r9, 0(r9)
	cmpwi r9, 1
	bne dontOverridePlayerRot2
	lis r9, lockedPlayerRotation@h
	ori r9, r9, lockedPlayerRotation@l
	lha r0, 0(r9)
dontOverridePlayerRot2:
	sth r0, 0xA(r1)
	blr

.global PlayerRotInjector3
PlayerRotInjector3:
	lis r9, isLockPlayerRotation@h
	ori r9, r9, isLockPlayerRotation@l
	lbz r9, 0(r9)
	cmpwi r9, 1
	bne dontOverridePlayerRot3
	lis r9, lockedPlayerRotation@h
	ori r9, r9, lockedPlayerRotation@l
	lha r7, 0(r9)
dontOverridePlayerRot3:
	sth r7, 0x12(r1)
	blr

# FOR SOME FUN
	# li r0, 0x6000
	# sth r0, 0x12(r1)
	# li r0, 0x4000
	# sth r0, 0x10(r1)
	# li r0, 0x8000
	# sth r0, 0x14(r1)


.global EFB_Crap
.extern EFBMagic2__Fv
.extern ContinueFromEFBMagic2
EFB_Crap:
	bl EFBMagic2__Fv
	lwz r3, 4(r30)
	b ContinueFromEFBMagic2

.extern drawFukidashi
.global NoFukidashi
NoFukidashi:
	lis r12, isNewerCredits@h
	ori r12, r12, isNewerCredits@l
	lbz r12, 0(r12)
	cmpwi r12, 1
	beqlr
	stwu r1, -0x10(r1)
	b drawFukidashi

.extern continueToSetAnim
.extern dontDoEndingWait
.global DisableEndingWaitAnim
DisableEndingWaitAnim:
	lis r12, isNewerCredits@h
	ori r12, r12, isNewerCredits@l
	lbz r12, 0(r12)
	cmpwi r12, 1
	beq dontDoAnim
	lwz r3, 0x2a64(r30)
	b continueToSetAnim

dontDoAnim:
	b dontDoEndingWait

.extern continueToGPThing
.extern dontDoGPThing
.global DisableGroundPoundThing
DisableGroundPoundThing:
	lis r12, isNewerCredits@h
	ori r12, r12, isNewerCredits@l
	lbz r12, 0(r12)
	cmpwi r12, 1
	beq dontDoGP
	lwz r3, 0x10d4(r31)
	b continueToGPThing

dontDoGP:
	b dontDoGPThing

.extern renderSJEffect
.extern dontRender
.global ReenableSpinJumpEffect
ReenableSpinJumpEffect:
	cmpwi r0, 0
	bne checkIfNewerCredits
	b renderSJEffect

checkIfNewerCredits:
	lis r12, isNewerCredits@h
	ori r12, r12, isNewerCredits@l
	lbz r12, 0(r12)
	cmpwi r12, 1
	bne notNewerCredits
	b renderSJEffect

notNewerCredits:
	b dontRender

.extern continueSpawnPlayersCredits
.extern normalSpawnPlayers
.global DisableEntranceHardcode
DisableEntranceHardcode:
	cmpwi r31, 0x17 # Boss standing entrance
	beq newerCreditsEntry
	lbz r4, 0x120e(r30)
	b continueSpawnPlayersCredits

newerCreditsEntry:
	b normalSpawnPlayers

.data
.global CameraLockEnabled
.global isLockPlayerRotation
.global CameraLockPosition
.global lockedPlayerRotation
.global isNewerCredits
CameraLockEnabled: .byte 0
isLockPlayerRotation: .byte 0
lockedPlayerRotation: .short 0
isNewerCredits: .byte 0
.align 4
CameraLockPosition: .float 0.0, 0.0