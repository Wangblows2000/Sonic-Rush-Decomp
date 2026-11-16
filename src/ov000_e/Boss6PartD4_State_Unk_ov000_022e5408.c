#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x348 - 0x20C];
    s32 unk348;
    s32 unk34C;
    s32 unk350;
    s32 unk354;
    u8 pad358[0x360 - 0x358];
    s32 unk360;
    s32 unk364;
    u32 unk368;
} UnkStruct_022e5408;

extern UnkStruct_StageObj *gPlayers;

static inline s32 Abs(s32 x) {
    if (x < 0)
        x = -x;
    return x;
}

void Boss6PartD4_State_Unk_ov000_022e5408(UnkStruct_022e5408 *s) {
    s32 v;
    s32 a;

    s->unk348 += s->unk34C;
    a = s->unk350 < 0 ? -s->unk350 : s->unk350;
    v = s->unk348 < 0 ? -s->unk348 : s->unk348;
    if (v > a)
        s->unk348 = s->unk350;
    s->base.unk18 += s->unk348 >> 4;
    s->base.unk14 += s->unk354 >> 4;
    if ((s->unk368 & 1) || Abs(gPlayers->unk18 - (s->base.unk18 + s->unk360)) > 0x3200) {
        s->base.coll0.unkA &= ~4;
        s->base.coll1.unkA &= ~4;
    } else {
        s->base.coll0.unkA |= 4;
        s->base.coll1.unkA |= 4;
    }
    s->unk364++;
    if (s->unk364 < 0x1f)
        return;
    s->base.unk54 |= 8;
    s->base.unk54 |= 2;
    s->base.unk1F4 |= 0x10;
    s->base.coll0.unkA |= 0x1000;
    s->base.coll1.unkA |= 0x1000;
}
