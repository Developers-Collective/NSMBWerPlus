#include <game.h>

struct daEnGakeNoko_c : public dEn_c {
	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c model;
	m3d::anmChr_c anmChr;
	nw4r::g3d::ResAnmTexPat resAnmTexPat;
	m3d::anmTexPat_c anmTexPat;
	u32 _5EC;
	int countdown;
	u8 gap[0x12C];

	USING_STATES(daEnGakeNoko_c);
	REF_NINTENDO_STATE(FoolMove);
	REF_NINTENDO_STATE(Move);
	REF_NINTENDO_STATE(Air);
	REF_NINTENDO_STATE(Awake);
	REF_NINTENDO_STATE(Fall);
	REF_NINTENDO_STATE(Fall2);
	REF_NINTENDO_STATE(Hang);
	REF_NINTENDO_STATE(DieFumi);
};

const ActivePhysics::Info GakeNokoColl = {
    0.0f,  // float xDistToCenter;
    11.0f,  // float yDistToCenter;
    8.0f,  // float xDistToEdge;
    11.0f,  // float yDistToEdge;
    3,  // u8 category1;
    0,  // u8 category2;
    0x4F,  // u32 bitfield1;
    0xFFBAFFFE,  // u32 bitfield2;
    0,  // u16 unkShort1C;
    &dEn_c::collisionCallback  // Callback callback;
};

int EN_GAKE_NOKO_ctor_new(daEnGakeNoko_c *self) {								//b @ 0x80A025A0
	self->allocator.link(-1, mHeap::gameHeaps[0], 0, 0x20);
	self->resFile.data = getResource("nokonokoB", "g3d/nokonokoB.brres");

	nw4r::g3d::ResMdl resMdl = self->resFile.GetResMdl("nokonokoB");
	self->model.setup(resMdl, &self->allocator, 0x7FFF, 1, 0);
	SetupTextures_Enemy(&self->model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = self->resFile.GetResAnmChr("net_walk2");
	self->anmChr.setup(resMdl, resAnmChr, &self->allocator, 0);

	self->resAnmTexPat = self->resFile.GetResAnmTexPat("nokonokoB");
	self->anmTexPat.setup(resMdl, self->resAnmTexPat, &self->allocator, 0, 1);
	self->anmTexPat.bindEntry(&self->model, &self->resAnmTexPat, 0, 1);

	self->allocator.unlink();

	// Nybble 10: green (0) / red (1)
	if ((self->settings >> 8) & 1) {
		self->doStateChange(&daEnGakeNoko_c::StateID_Move);
	} else {
		self->doStateChange(&daEnGakeNoko_c::StateID_FoolMove);
	}
	if ((self->settings >> 8) & 1) {
		self->anmTexPat.setEntryByte34(1, 0);
		self->model.bindAnim(&self->anmTexPat, 0.0);
		self->anmTexPat.setFrameForEntry(1.0, 0);
	} else {
		self->anmTexPat.setFrameForEntry(0.0, 0);
	}

	// Nybble 11: initial direction: right (0) / left (1)
	if ((self->settings >> 4) & 1) {
		// (Logic copied from how the Move state switches directions
		// at the end of a ledge -- basic block at 80A02E00 in PALv1)
		self->direction ^= 1;
		self->speed.x = -self->speed.x;
		self->anmChr.playState = 2;
	}

	// Nybble 12: if 1, vertical positioning is improved
	if (self->settings & 1) {
		self->pos.y += 16.0;
	}

	// ActivePhysics for 2021
	self->aPhysics.initWithStruct(self, (ActivePhysics::Info *)&GakeNokoColl);
	self->aPhysics.addToList();

	self->rot.y = 0x8000;  // Face the wall

	return 1;
}


// Edit some Y-position-related constants in various states if nybble 12 is enabled
float twenty_four_or_alt_f[2] = { 24.0f, 8.0f };
float thirty_one_or_alt_f[2] = { 31.0f, 15.0f };
