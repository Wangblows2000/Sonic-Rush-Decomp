#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D4 - 0x20C];
    s32 unk3D4;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_Boss *unk214;
} UnkStruct_022ec568;

void Boss7_SetRivalAnimation(UnkStruct_Boss *p, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_Boss *p, s32 a, s32 b);
void Boss7_State_None(void);

void Boss7_State_ClashLostRivalSkid(UnkStruct_022ec568 *w) {
    UnkStruct_Boss *p = w->unk214;
    s32 st;

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
    st = w->unk214->unk3D4;
    if (st == 5 && (p->base.unk5C & 8)) {
        Boss7_SetRivalAnimation(p, 6);
        p->base.unk5C |= 4;
    } else if (p->base.unk2E < -0x40 && st == 6 && (p->base.unk5C & 8)) {
        Boss7_SetRivalAnimation(p, 7);
        p->base.unk5C &= ~4;
    } else if (st == 7 && (p->base.unk5C & 8)) {
        Boss7_SetRivalAnimation(p, 0x3c);
        p->base.unk5C |= 4;
    }
    Boss7_SetCollidersEnabled(p, 0, 0);
    if (w->unk214->unk3D4 != 0x3c)
        return;
    if (p->base.unk2E == 0)
        w->base.unk68 = Boss7_State_None;
}
