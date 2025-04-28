#include <common.h>
#include <game.h>
#include <profile.h>

// TODO: make "No Deactivation"

struct BgActor {
	u16 def_id;		// 0x00
	u16 x;			// 0x02
	u16 y;			// 0x04
	u8 layer;		// 0x06
	u8 EXTRA_off;	// 0x07
	u32 actor_id;	// 0x08
};

struct BgActorDef {
	u32 tilenum;
	u16 actor;
	u8 _06[2];
	float x;
	float y;
	float z;
	float width;
	float height;
	u32 extra_var;
};

struct dBgActorManager_c {
	u32 vtable;		// 0x00
	u8 _04[0x34];	// 0x04
	BgActor *array;	// 0x38
	u32 count;		// 0x3C
	u32 type;		// 0x40
};

extern dBgActorManager_c *dBgActorManager;

extern BgActorDef *BgActorDefs;

struct BG_GM_hax {
	u8 _00[0x8FE64];
	float _0x8FE64;
	float _0x8FE68;
	float _0x8FE6C;
	float _0x8FE70;
};

extern BG_GM_hax *BG_GM_ptr;

#define LINEGOD_FUNC_ACTIVATE	0
#define LINEGOD_FUNC_DEACTIVATE	1

class LineGod : public dStageActor_c {
	int onCreate();
	int onExecute();

	u64 eventFlag;
	u8 func;
	u8 width;
	u8 height;
	u8 lastEvState;
	BgActor *ac[8];

	public: static dActor_c *build();

	void BuildList();
	bool AppendToList(BgActor *ac);
	void Update();
};

dActor_c *LineGod::build() {
	void *buffer = AllocFromGameHeap1(sizeof(LineGod));
	return new(buffer) LineGod;
}

const SpriteData LineGodSpriteData = { ProfileId::AC_LINEGOD, 0, 0, 0 , 0, 0x10, 0x10, 0, 0, 0, 0, 8};
Profile LineGodProfile(&LineGod::build, SpriteId::AC_LINEGOD, &LineGodSpriteData, ProfileId::RIVER_MGR, ProfileId::AC_LINEGOD, "AC_LINEGOD");

u16 *GetPointerToTile(BG_GM_hax *self, u16 x, u16 y, u16 layer, short *blockID_p, bool unused);

int LineGod::onCreate() {
	char eventNum = (this->settings >> 24) & 0xFF;
	this->eventFlag = (u64)1 << (eventNum - 1);

	this->func = (this->settings) & 1;
	this->width = (this->settings >> 4) & 0xF;
	this->width = (this->settings >> 8) & 0xF;

	this->lastEvState = 0xFF;

	BuildList();
	onExecute();

	return true;
}

int LineGod::onExecute() {
	u8 newEvState = 0;
	if (dFlagMgr_c::instance->flags & this->eventFlag)
		newEvState = 1;
	
	if (newEvState == this->lastEvState)
		return;
	
	u16 x_bias = (BG_GM_ptr->_0x8FE64 / 16);
	u16 y_bias = -(BG_GM_ptr->_0x8FE6C / 16);
	
	
	u8 offState;
	if (this->func == LINEGOD_FUNC_ACTIVATE)
		offState = (newEvState == 1) ? 1 : 0;
	else
		offState = (newEvState == 1) ? 0 : 1;
	
	
	for (int i = 0; i < 8; i++) {
		if (this->ac[i] != 0) {
			BgActor *ac = this->ac[i];
			
			
			ac->EXTRA_off = offState;
			if (offState == 1 && ac->actor_id != 0) {
				fBase_c *assoc_ac = searchById(ac->actor_id);
				if (assoc_ac != 0)
					assoc_ac->Delete();
				ac->actor_id = 0;
			}
			
			u16 *tile = GetPointerToTile(BG_GM_ptr, (ac->x + x_bias) * 16, (ac->y + y_bias) * 16, 0, 0, 0);
			if (offState == 1)
				*tile = 0;
			else
				*tile = BgActorDefs[ac->def_id].tilenum;
			
		}
	}
	
	
	
	this->lastEvState = newEvState;
	return true;
}

void LineGod::BuildList() {
	for (int clearIdx = 0; clearIdx < 8; clearIdx++) {
		this->ac[clearIdx] = 0;
	}

	float gLeft = this->pos.x - (BG_GM_ptr->_0x8FE64 - fmod(BG_GM_ptr->_0x8FE64, 16));
	float gTop = this->pos.y - (BG_GM_ptr->_0x8FE6C - fmod(BG_GM_ptr->_0x8FE6C, 16));

	// 1 unit padding to avoid catching stuff that is not in our rectangle
	Vec grect1 = (Vec){
		gLeft + 1, gTop - (this->height * 16) + 1, 0
	};

	Vec grect2 = (Vec){
		gLeft + (this->width * 16) - 1, gTop - 1, 0
	};

	for (int i = 0; i < dBgActorManager->count; i++) {
		BgActor *ac = &dBgActorManager->array[i];

		// the Def width/heights are padded with 8 units on each side
		// except for one of the steep slopes, which differs for no reason

		BgActorDef *def = &BgActorDefs[ac->def_id];
		float aXCentre = (ac->x * 16) + def->x;
		float aYCentre = (-ac->y * 16) + def->y;

		float xDistToCentre = (def->width - 16) / 2;
		float yDistToCentre = (def->height - 16) / 2;

		Vec arect1 = (Vec){
			aXCentre - xDistToCentre, aYCentre - yDistToCentre, 0
		};
		
		Vec arect2 = (Vec){
			aXCentre + xDistToCentre, aYCentre + yDistToCentre, 0
		};

		if (RectanglesOverlap(&arect1, &arect2, &grect1, &grect2))
			AppendToList(ac);
	}
}

bool LineGod::AppendToList(BgActor *ac) {
	for (int search = 0; search < 8; search++) {
		if (this->ac[search] == 0) {
			this->ac[search] = ac;
			return true;
		}
	}

	return false;
}