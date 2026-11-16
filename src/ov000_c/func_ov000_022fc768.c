#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x352 - 0x20C];
    u16 unk352;
    u8 pad354[0x3B4 - 0x354];
    s16 unk3B4;
} UnkStruct_Player;

void func_0206d8bc(UnkStruct_Coll *a, UnkStruct_Coll *b);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void func_ov000_022fc768(UnkStruct_Coll *a, UnkStruct_Coll *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_Player *p = b->unk18;

    if (w == NULL)
        return;
    if (p == NULL)
        return;
    if (p->base.unk0 != 1)
        return;
    if (p->unk3B4 != 0) {
        func_0206d8bc(a, b);
        return;
    }
    p->base.unk34 = ABS(p->base.unk34);
    if (p->base.unk34 < 0x500)
        p->base.unk34 = 0x500;
    if (p->base.unk2E != 0) {
        p->base.unk2E = ABS(p->base.unk2E);
        if (p->base.unk2E < 0x500)
            p->base.unk2E = 0x500;
    }
    if (w->unk10 > p->base.unk10) {
        p->base.unk34 = -p->base.unk34;
        p->base.unk2E = -p->base.unk2E;
    }
    if (w->unk5 != 0) {
        p->base.unk34 = -p->base.unk34;
        p->base.unk2E = -p->base.unk2E;
    }
    if (p->base.unk30 > 0)
        p->base.unk30 = 0;
    p->unk352 = 0x10;
    p->base.unk62 = 0x44;
    w->unk62 = 0x44;
}
