#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
    u8 pad22[2];
    struct {
        u16 b0 : 5;
        u16 b5 : 11;
    } unk24;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x230 - 0x20C];
    u16 unk230;
    u16 unk232;
    s32 unk234;
    u8 pad238[0x23C - 0x238];
    void *unk23C;
    u8 pad240[0x248 - 0x240];
    void *unk248;
} UnkStruct_022ed6c4;

extern UnkStruct_0208922c gDisplayMain;

void NNS_G3dGlbMaterialColorDiffAmb(s32 a, s32 b, s32 c);
void Boss7_State_ClashStartApproach(void);

void Boss7_State_ClashFlash(UnkStruct_022ed6c4 *s) {
    u16 v;
    u16 c;

    s->unk232 -= 0x40;
    if ((s16)s->unk232 <= 0)
        s->unk232 = 0;
    gDisplayMain.unk24.b0 = (u16)((s->unk232 + 0xff) >> 8);
    s->unk234 -= 0x100;
    if (s->unk234 <= 0x400)
        s->unk234 = 0x400;
    v = (s->unk234 + 0xff) >> 8;
    c = v | (v << 5) | (v << 10);
    NNS_G3dGlbMaterialColorDiffAmb(c, c, 0);
    if (s->unk230++ > 0x78)
        s->base.unk68 = Boss7_State_ClashStartApproach;
}
