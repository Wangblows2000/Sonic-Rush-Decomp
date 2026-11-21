#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    u8 pad18[0x20 - 0x18];
    s32 unk20;
    u8 pad24[0x1E0 - 0x24];
    UnkStruct_ObjParam *unk1E0;
} UnkStruct_023127e8;

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

void Piston_InitOscillationPhase(UnkStruct_023127e8 *w) {
    u16 t;
    s32 amp;
    s32 y;
    s32 base;

    w->unk20 = (w->unk14 >> 8) + w->unk1E0->unk7 + (w->unk1E0->unk9 >> 1);
    if (w->unk1E0->unk9 == 0)
        return;
    amp = w->unk1E0->unk9 >> 1;
    base = w->unk20;
    y = w->unk14 >> 8;
    t = 0x300;
    do {
        if (base + ((amp * SIN((u16)(t << 6))) >> 12) > y)
            break;
        t -= 4;
    } while (t > 0x100);
    w->unkC = t;
    w->unkC -= (s16)(((w->unk1E0->unk4 & 0x30) >> 4) << 8);
    w->unkC &= 0x3fff;
}
