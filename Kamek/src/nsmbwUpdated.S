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

.global turnIcicleIntoIceballYoshiEat
turnIcicleIntoIceballYoshiEat:
    stfs f0, 0x31c(r28)
    li r4, 5
    stb r4, 0x36d(r28)
    blr


.extern FUN_8085f5c0	#0x8085f5c0
.extern fixJumbo1return	#0x80860074
.extern fixJumbo2return	#0x808600d8
.global fixJumbo1
fixJumbo1:
	bl FUN_8085f5c0
	b fixJumbo1return

.global fixJumbo2
fixJumbo2:
	bl FUN_8085f5c0
	b fixJumbo2return


.global fixLightmapsForDolphin
fixLightmapsForDolphin:
	li r4, 5
    stb r4, 0xc(r3)
    li r4, 1
    stb r4, 0xd(r3)
    li r4, 0
    blr


.global fixEventControlledCameras
fixEventControlledCameras:
    li r31, 0xff
    stb r31, 0x20(r6)
    li r31, 0
    blr