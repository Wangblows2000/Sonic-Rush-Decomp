#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x438];
    u16 unk438;
} UnkStruct_Ctl;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_Ctl *unk218;
} UnkStruct_02369b84;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    u16 unk210;
    u8 pad212[2];
    u16 unk214;
    u8 pad216[2];
    s32 unk218;
    u8 pad21C[0x35C - 0x21C];
    s32 unk35C;
    u8 pad360[0x4A0 - 0x360];
    s32 unk4A0;
    u8 pad4A4[0x4A8 - 0x4A4];
    u8 unk4A8[4];
} UnkStruct_022f569c;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369b84 *data_ov000_02369b84;
extern s16 data_ov000_02364090[][10];

void func_0201c070(void *a, s32 b);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void BossFPartDE_State_Unk_ov000_022f569c(UnkStruct_022f569c *w) {
    BOOL far = FALSE;

    w->base.unk30 -= data_ov000_02364090[GetLevel()][w->unk214];
    w->unk210 -= w->base.unk30 << 2;
    w->unk218 += w->base.unk30;
    if (w->base.unk10 - w->unk218 > 0xb400)
        far = TRUE;
    if (w->unk218 < 0)
        w->unk218 = 0;
    w->unk35C -= w->base.unk30;
    if (w->unk35C - w->base.unk10 > 0xb400)
        far = TRUE;
    if (w->unk35C > 0x34000)
        w->unk35C = 0x34000;
    if (far && w->unk4A0 != 0) {
        w->unk4A0 = 0;
        w->base.coll0.unkA &= ~4;
        func_0201c070(w->unk4A8, 0x3c);
    }
    if (w->base.unk14 < 0x1a000) {
        data_ov000_02369b84->unk218->unk438--;
        w->base.unk54 |= 4;
    }
}
