#include <game.h>
#include <profile.h>

const char* RYOMRes1ArcNameList [] = { "RYOMRes1", NULL };
const char* RYOMRes2ArcNameList [] = { "RYOMRes2", NULL };
const char* RYOMRes3ArcNameList [] = { "RYOMRes3", NULL };
const char* RYOMRes4ArcNameList [] = { "RYOMRes4", NULL };
const char* RYOMRes5ArcNameList [] = { "RYOMRes5", NULL };

class dRYOMRes1_c : public dEn_c {
public:
	static dActor_c* build();
	mHeapAllocator_c allocator;

};

dActor_c* dRYOMRes1_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRYOMRes1_c));
	return new(buffer) dRYOMRes1_c;
}

const SpriteData RYOMRes1SpriteData = 
{ ProfileId::RYOMRes1, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes1Profile(&dRYOMRes1_c::build, SpriteId::RYOMRes1, &RYOMRes1SpriteData, ProfileId::RYOMRes1, ProfileId::RYOMRes1, "RYOMRes1", RYOMRes1ArcNameList);

class dRYOMRes2_c : public dEn_c {
public:
	static dActor_c* build();
	mHeapAllocator_c allocator;

};

dActor_c* dRYOMRes2_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRYOMRes2_c));
	return new(buffer) dRYOMRes2_c;
}

const SpriteData RYOMRes2SpriteData = 
{ ProfileId::RYOMRes2, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes2Profile(&dRYOMRes2_c::build, SpriteId::RYOMRes2, &RYOMRes2SpriteData, ProfileId::RYOMRes2, ProfileId::RYOMRes2, "RYOMRes2", RYOMRes2ArcNameList);

class dRYOMRes3_c : public dEn_c {
public:
	static dActor_c* build();
	mHeapAllocator_c allocator;

};

dActor_c* dRYOMRes3_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRYOMRes3_c));
	return new(buffer) dRYOMRes3_c;
}

const SpriteData RYOMRes3SpriteData = 
{ ProfileId::RYOMRes3, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes3Profile(&dRYOMRes3_c::build, SpriteId::RYOMRes3, &RYOMRes3SpriteData, ProfileId::RYOMRes3, ProfileId::RYOMRes3, "RYOMRes3", RYOMRes3ArcNameList);

class dRYOMRes4_c : public dEn_c {
public:
	static dActor_c* build();
	mHeapAllocator_c allocator;

};

dActor_c* dRYOMRes4_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRYOMRes4_c));
	return new(buffer) dRYOMRes4_c;
}

const SpriteData RYOMRes4SpriteData = 
{ ProfileId::RYOMRes4, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes4Profile(&dRYOMRes4_c::build, SpriteId::RYOMRes4, &RYOMRes4SpriteData, ProfileId::RYOMRes4, ProfileId::RYOMRes4, "RYOMRes4", RYOMRes4ArcNameList);

class dRYOMRes5_c : public dEn_c {
public:
	static dActor_c* build();
	mHeapAllocator_c allocator;

};

dActor_c* dRYOMRes5_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRYOMRes5_c));
	return new(buffer) dRYOMRes5_c;
}

const SpriteData RYOMRes5SpriteData = 
{ ProfileId::RYOMRes5, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile RYOMRes5Profile(&dRYOMRes5_c::build, SpriteId::RYOMRes5, &RYOMRes5SpriteData, ProfileId::RYOMRes5, ProfileId::RYOMRes5, "RYOMRes5", RYOMRes5ArcNameList);