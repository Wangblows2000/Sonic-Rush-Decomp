#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[4];
    u32 unk210;
} UnkStruct_Debris;

UnkStruct_Debris *EffFire01_3D_Create(UnkStruct_StageObj *w, s16 x, s16 y, s16 z);

void func_ov000_022eb3c4(UnkStruct_StageObj *w) {
    s16 dx, dy, dz;
    UnkStruct_Debris *o;

    dx = (Rand() & 7) - 8;
    dx += w->unk2E >> 8;
    dy = (Rand() & 0xf) - 0xc;
    dy += w->unk30 >> 8;
    dz = (Rand() & 0x1f) - 10;
    o = EffFire01_3D_Create(w, dx, dy, dz);

    if (w->unk5C & 1)
        o->base.unk2E = (Rand() & 0x3f) + 0x1c0;
    else
        o->base.unk2E = -((Rand() & 0x3f) + 0x140);
    o->base.unk30 = -((Rand() & 0x1f) + 0x40);
    o->base.unk58 |= 0x80;
    o->base.unk40 = -((Rand() & 0x1f) + 0x10);
    o->unk210 = (o->unk210 & ~0x3f000000) | 0x1000000;
}
