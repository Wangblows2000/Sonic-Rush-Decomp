#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    u16 unk214;
    u8 pad216[2];
    s32 unk218;
    s32 unk21C;
    s32 unk220;
    u8 pad224[0x35C - 0x224];
    s32 unk35C;
    s32 unk360;
    s32 unk364;
    u8 pad368[0x4A0 - 0x368];
    s32 unk4A0;
    s32 unk4A4;
} UnkStruct_022f58cc;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_StageObj *gPlayers;
extern s16 data_ov000_02364068[][10];

void NNS_G3dGlbPolygonAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void BossFPartDE_State_Unk_ov000_022f58ac(void);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void BossFPartDE_State_Unk_ov000_022f58cc(UnkStruct_022f58cc *w) {
    s32 margin, off, v, a;
    BOOL dir;
    UnkStruct_StageObj *pl;

    w->base.unk30 = 0;
    w->base.unk10 = gPlayers->unk10;
    w->base.unk14 = 0x17000;
    w->base.unk18 = gPlayers->unk18;
    w->base.unk30 = 0xa00;
    margin = (data_ov000_02364068[GetLevel()][w->unk214] >> 8) + 0x20;
    off = (Rand() & 7) << 10;
    off += data_ov000_02364068[GetLevel()][w->unk214];
    pl = gPlayers;
    if (pl->unk10 < margin << 8) {
        dir = FALSE;
    } else if ((0x300 - margin) << 8 < pl->unk10) {
        dir = TRUE;
    } else {
        v = pl->unk2E;
        a = v < 0 ? -v : v;
        if (a > 0x280) {
            if (v > 0)
                dir = (Rand() & 7) < 6;
            else
                dir = (Rand() & 7) >= 6;
        } else {
            dir = !(Rand() & 1);
        }
    }
    if (dir)
        w->base.unk10 -= off;
    else
        w->base.unk10 += off;
    w->unk218 = w->base.unk10 - 0x4000;
    w->unk21C = 0x30000;
    w->unk220 = 0;
    w->unk35C = w->base.unk10 + 0x4000;
    w->unk360 = 0x30000;
    w->unk364 = 0;
    if (w->unk218 < 0)
        w->unk218 = 0;
    if (w->unk35C > 0x34000)
        w->unk35C = 0x34000;
    NNS_G3dGlbPolygonAttr(0, 0, 2, 0, 0xc, 0);
    w->unk4A0 = 1;
    w->unk4A4 = 1;
    w->base.unk68 = BossFPartDE_State_Unk_ov000_022f58ac;
}
