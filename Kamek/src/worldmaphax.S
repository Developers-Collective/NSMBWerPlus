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

.extern returnFromScaleTable
.global newShadowScales
newShadowScales:
    lis r4, newScalesTable@h
    subi r4, r4, newScalesTable@l
	add r4, r4, r0
	lfs f2, 0x0(r4)
    lfs f3,0x4(r4)
    lfs f4, 0x8(r4)
    b returnFromScaleTable

.extern returnFromScaleTable2
.global newShadowScales2
newShadowScales2:
	lis r5, newScalesTable@h
    subi r5, r5, newScalesTable@l
	add r4, r5, r0
	lfs f2, 0x0(r4)
    lfs f3,0x4(r4)
    lfs f4, 0x8(r4)
	b returnFromScaleTable2

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
	.float 0.6, 1.0, 0.6
	.float 0.8, 1.0, 0.8
	.float 0.8, 1.0, 0.8
	.float 0.4, 1.0, 0.4
	.float 0.8, 1.0, 0.8
	.float 0.8, 1.0, 0.8
	.float 0.8, 1.0, 0.8
	.float 0.8, 1.0, 0.8 #NEW: scales for hammer