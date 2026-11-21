#include <game/Stage/stageobj_alt.h>

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

void DecGroup35_SpawnPieceBurst(s32 x, s32 y);
void DecGroup35_CreateFallingDebris(s32 x, s32 y, s32 a, s32 b, s32 c);
void *EffWaterGmk_CreateSplash(UnkStruct_02322638_O *o, s32 a, s32 b);

void DecGroup35_OnHit(UnkStruct_02322638_C *a, UnkStruct_02322638_C *b) {
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
            DecGroup35_SpawnPieceBurst(w->unk10 + (0x1fff - (Rand() & 0x3ffe)), w->unk14);
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
            DecGroup35_CreateFallingDebris(w->unk10 + (0x1fff - (Rand() & 0x3ffe)), w->unk14, 0, 0, 0);
        }
        if (!(Rand() & 1)) {
            DecGroup35_CreateFallingDebris(w->unk10 + (0x1fff - (Rand() & 0x3ffe)), w->unk14, 0, 0, 1);
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
        e = EffWaterGmk_CreateSplash(o, 0, 0x10);
        w->unk54 |= 2;
        if (w->unk184->unk2 == 0x52) {
            StageObj_SetOAMOrder(e, 9);
            StageObj_SetOAMPriority(e, 1);
        }
        break;
    }
}
