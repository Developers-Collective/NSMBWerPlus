.text
.align 4
.extern ContinueFromClownCarDetectAdjacent
.extern calculateAdjacentCollision__14collisionMgr_cFPf
.global ClownCarDetectAdjacents
ClownCarDetectAdjacents:
	addi r3, r30, 0x1EC
	li r4, 0
	bl calculateAdjacentCollision__14collisionMgr_cFPf
	cmpwi r3, 0
	beq notAdjacent

	# what direction are we in?
	lfs f1, 0xE8(r30) # x speed
	lis r4, zeroFloat@h
	ori r4, r4, zeroFloat@l
	lfs f2, 0(r4)

	lbz r4, 0x348(r30)

	cmpwi r4, 1
	beq isLeft

	# RIGHT
	fcmpo cr0, f1, f2 # xspeed > 0
	blt notAdjacent
	stfs f2, 0xE8(r30)
	b notAdjacent

isLeft:
	# LEFT
	fcmpo cr0, f1, f2 # xspeed < 0
	bgt notAdjacent
	stfs f2, 0xE8(r30)

notAdjacent:
	# END HERE

	mr r3, r30
	b ContinueFromClownCarDetectAdjacent

.global ClownCarFloor
ClownCarFloor:
	lbz r12, 0x5(r31) 		#settings
	andi. r12, r12, 0x2 	#bit 31
	cmpwi r12, 1			#if bit 31 is not set
	blt OriginalFloor
	blr

OriginalFloor:
	stfs f0,0xb0(r31)
	blr

.global ClownCarWalls
ClownCarWalls:
	lis r12, originalWallFloat@h
	ori r12, r12, originalWallFloat@l
	lfs f2, 0(r12)

	lbz r12, 0x5(r3) 		#settings
	andi. r12, r12, 0x2 	#bit 31
	cmpwi r12, 0			#if bit 31 is set
	bgt NewWall
	blr

NewWall:
	lis r12, newWallFloat@h
	ori r12, r12, newWallFloat@l
	lfs f2, 0(r12)
	blr


.data
.align 4
zeroFloat: .float 0.0
originalWallFloat: .float 224.0
newWallFloat: .float 40000.0
