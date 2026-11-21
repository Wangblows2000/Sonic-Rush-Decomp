#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x224 - 0x210];
    s32 unk224;
    s16 unk228;
    s16 unk22A;
    s16 unk22C;
} UnkStruct_022edcd8;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3B2 - 0x20C];
    s16 unk3B2;
} UnkStruct_Player;

extern UnkStruct_Player *gPlayers;

void Boss7_State_EndFight(void);

void Boss7_State_Fight(UnkStruct_022edcd8 *w) {
    if (w->unk20C & 0x20) {
        s16 m = w->unk228;
        if (gPlayers->unk3B2 > 0)
            m >>= 2;
        w->unk22A += w->unk22C;
        w->unk22A = w->unk22A < 0 ? 0 : (w->unk22A > m ? m : w->unk22A);
        if (w->unk224 < gPlayers->base.unk10)
            gPlayers->base.unk36 = -w->unk22A;
        else
            gPlayers->base.unk36 = w->unk22A;
    }
    if (w->unk20C & 2)
        w->base.unk68 = Boss7_State_EndFight;
}
