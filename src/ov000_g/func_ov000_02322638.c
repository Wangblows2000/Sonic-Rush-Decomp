#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x28 - 0x18];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x54 - 0x2C];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x184 - 0x5C];
    UnkStruct_ObjParam *unk184;
} UnkStruct_02322638_O;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_02322638_O *unk18;
} UnkStruct_02322638_C;

void func_ov000_0232233c(s32 x, s32 y);
void func_ov000_02321f18(s32 x, s32 y, s32 a, s32 b, s32 c);
void *func_ov000_02328fb8(UnkStruct_02322638_O *o, s32 a, s32 b);

void func_ov000_02322638(UnkStruct_02322638_C *a, UnkStruct_02322638_C *b) {
    UnkStruct_02322638_O *w = a->unk18;
    UnkStruct_02322638_O *o = b->unk18;
    s32 v;
    void *e;

    if (w == NULL)
        return;
    switch (w->unk184->unk2) {
    case 0x35:
        if (!(o->unk58 & 1))
            return;
        v = o->unk28;
        if (v < 0)
            v = -v;
        if (v <= 0x100)
            return;
        if (!(Rand() & 1)) {
            func_ov000_0232233c(w->unk10 + (0x1fff - (Rand() & 0x3ffe)), w->unk14);
        }
        w->unk54 |= 2;
        break;
    case 0x36:
        if (!(o->unk58 & 1))
            return;
        v = o->unk28;
        if (v < 0)
            v = -v;
        if (v <= 0x180)
            return;
        if (!(Rand() & 1)) {
            func_ov000_02321f18(w->unk10 + (0x1fff - (Rand() & 0x3ffe)), w->unk14, 0, 0, 0);
        }
        if (!(Rand() & 1)) {
            func_ov000_02321f18(w->unk10 + (0x1fff - (Rand() & 0x3ffe)), w->unk14, 0, 0, 1);
        }
        w->unk54 |= 2;
        break;
    case 0x51:
    case 0x52:
        if (!(o->unk58 & 0x400001))
            return;
        v = o->unk28;
        if (v < 0)
            v = -v;
        if (v <= 0x80 && o->unk2A == 0)
            return;
        e = func_ov000_02328fb8(o, 0, 0x10);
        w->unk54 |= 2;
        if (w->unk184->unk2 == 0x52) {
            StageObj_SetOAMOrder(e, 9);
            StageObj_SetOAMPriority(e, 1);
        }
        break;
    }
}
