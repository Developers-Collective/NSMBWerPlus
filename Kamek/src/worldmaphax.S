.text
#include <asm_setup.S>

.align 4
.extern spawnMapManager__Fv

.extern aRouteinfo_1, aCollectioncoincoursesort, aCobcastle_1, aCobcourse_0, aCobkinokored_3, aCobkinoko1up_4, aCobkinokosta_3, aCobstart_0, aCobanchor_0, aCobtoride_1, aCobghost_1, aCobcannon_1, aCobkoopaship_5, aCharacter_s_12, aCobsurrender_0, aCobkinopio_1

.global saveForWM
saveForWM:
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x40(sp)
	stw r3, 0x3C(sp)
	stw r4, 0x38(sp)
	stw r5, 0x34(sp)
	stw r6, 0x30(sp)
	stw r24, 0x2C(sp)
	stw r25, 0x28(sp)
	stw r26, 0x24(sp)
	stw r27, 0x20(sp)
	stw r28, 0x1C(sp)
	stw r29, 0x18(sp)
	stw r30, 0x14(sp)
	stw r31, 0x10(sp)
    stw r13, 0x0C(sp)
	bl spawnMapManager__Fv
	cmpwi	r3, 1
	b		restoreForWM

.global restoreForWM
restoreForWM:
    lwz r13, 0x0C(sp)
	lwz r31, 0x10(sp)
	lwz r30, 0x14(sp)
	lwz r29, 0x18(sp)
	lwz r28, 0x1C(sp)
	lwz r27, 0x20(sp)
	lwz r26, 0x24(sp)
	lwz r25, 0x28(sp)
	lwz r24, 0x2C(sp)
	lwz r6, 0x30(sp)
	lwz r5, 0x34(sp)
	lwz r4, 0x38(sp)
	lwz r3, 0x3C(sp)
	lwz r0, 0x40(sp)
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40
	blr

.global useNewFileListR26
useNewFileListR26:
	lis r26, newAlwaysLoadedList@h
	ori r26, r26, newAlwaysLoadedList@l
	blr

.global useNewFileListR31
useNewFileListR31:
	lis r31, newAlwaysLoadedList@h
	ori r31, r31, newAlwaysLoadedList@l
	blr

.extern FUN_800f6920
.extern setSRT__21dPlayerModelHandler_cF7Point3d6S16Vec7Point3d
.extern update__21dPlayerModelHandler_cFv
.global newShadowScales
newShadowScales:
	stwu r1, -0x30(r1)
	mfspr r0, LR
    lis r4, newScalesTable@h
    stw r0, 0x34(r1)
    subi r4, r4, newScalesTable@l
    stw r31, 0x2c(r1)
    or r31, r3, r3
    lwz r5, 0x1d0(r3)
	lis r12, randomFloat@h
	ori r12, r12, randomFloat@l
    lfs f1, 0x0(r12)
	lbz r0, 0x152(r5)
    mulli r0, r0, 0xc
    add r4, r4, r0
    lfs f2, 0x0(r4)
    lfs f3,0x4(r4)
    lfs f4, 0x8(r4)
    bl FUN_800f6920
    lfs f0, 0xb4(r31)
    addi r3, r31, 0x1cc
    lfs f1, 0xb0(r31)
    addi r4, r1, 0x1c
    lfs f2, 0xac(r31)
    addi r5, r1, 0x8
    stfs f2, 0x1c(r1)
    addi r6, r1, 0x10
    stfs f1, 0x20(r1)
    stfs f0, 0x24(r1)
    lfs f0, 0xe4(r31)
    lfs f1, 0xe0(r31)
    lfs f2, 0xdc(r31)
    stfs f2, 0x10(r1)
    stfs f1, 0x14(r1)
    stfs f0, 0x18(r1)
    lha r0, 0x100(r31)
    sth r0, 0x8(r1)
    lha r0, 0x102(r31)
    sth r0, 0xa(r1)
    lha r0, 0x104(r31)
    sth r0, 0xc(r1)
    bl setSRT__21dPlayerModelHandler_cF7Point3d6S16Vec7Point3d
    addi r3, r31, 0x1cc
    bl update__21dPlayerModelHandler_cFv
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtspr LR ,r0
    addi r1, r1, 0x30
    blr

.data
.global newAlwaysLoadedList
newAlwaysLoadedList:
	.long aRouteinfo_1
	.long aCollectioncoincoursesort
	.long aCobcastle_1
	.long aCobcourse_0
	.long aCobkinokored_3
	.long aCobkinoko1up_4
	.long aCobkinokosta_3
	.long aCobstart_0
	.long aCobanchor_0
	.long aCobtoride_1
	.long aCobghost_1
	.long aCobcannon_1
	.long aCobkoopaship_5
	.long aCharacter_s_12
	.long aCobsurrender_0
	.long aCobkinopio_1

.global newScalesTable
newScalesTable:
	.float 0.6
	.float 1.0
	.float 0.6
	.float 0.8
	.float 1.0
	.float 0.8
	.float 0.8
	.float 1.0
	.float 0.8
	.float 0.4
	.float 1.0
	.float 0.4
	.float 0.8
	.float 1.0
	.float 0.8
	.float 0.8
	.float 1.0
	.float 0.8
	.float 0.8
	.float 1.0
	.float 0.8
	.float 0.8 #NEW: scales for hammer
	.float 1.0
	.float 0.8

randomFloat: .float 0.5