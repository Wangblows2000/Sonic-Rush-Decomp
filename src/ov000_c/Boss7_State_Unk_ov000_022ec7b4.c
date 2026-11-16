#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x230 - 0x20C];
    u16 unk230;
} UnkStruct_022ec7b4;

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Player *gPlayers;

void Player_SetAnimation(UnkStruct_Player *p, s32 a);
void Boss7_State_Unk_ov000_022ec564(void);

void Boss7_State_Unk_ov000_022ec7b4(UnkStruct_022ec7b4 *w) {
    UnkStruct_Player *p = gPlayers;
    s16 st;

    w->unk230++;
    if (!(w->unk230 & 3) && gDisplayMain.unk48 > 0)
        gDisplayMain.unk48--;
    p->base.unk10 += p->base.unk2E;
    if (p->base.unk2E < 0) {
        if (-p->base.unk2E > 10)
            p->base.unk2E = p->base.unk2E + 10;
        else
            p->base.unk2E = 0;
    } else if (p->base.unk2E > 0) {
        if (p->base.unk2E > 10)
            p->base.unk2E = p->base.unk2E - 10;
        else
            p->base.unk2E = 0;
    }
    st = gPlayers->unk328;
    if (st == 5 && (p->base.unk5C & 8)) {
        Player_SetAnimation(p, 6);
        p->base.unk5C |= 4;
    } else if (p->base.unk2E < 0x40 && st == 6 && (p->base.unk5C & 8)) {
        Player_SetAnimation(p, 7);
        p->base.unk5C &= ~4;
    } else if (st == 7 && (p->base.unk5C & 8)) {
        Player_SetAnimation(p, 0x3c);
        p->base.unk5C |= 4;
    }
    if (gPlayers->unk328 != 0x3c)
        return;
    if (p->base.unk2E != 0)
        return;
    if (gDisplayMain.unk48 == 0)
        w->base.unk68 = Boss7_State_Unk_ov000_022ec564;
}
