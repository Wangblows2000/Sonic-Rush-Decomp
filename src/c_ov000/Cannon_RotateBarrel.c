#include <ov000_c/stageobj.h>

u16 ObjAngle_MoveToward16(u16 a, u16 b, s32 c);

void Cannon_RotateBarrel(UnkStruct_StageObj *w) {
    u16 base;
    s16 spd;
    s32 t;
    base = 0;
    spd = 0x100;
    if (w->unk1E0->unk4 & 8)
        spd <<= 1;
    if (w->unk1E0->unk4 & 4) {
        w->unk8 += spd;
        w->unk8 &= 0xffff;
        return;
    }
    base += (w->unk1E0->unk4 & 3) << 14;
    if (w->unk1F4 & 2) {
        t = base + 0x2000;
        w->unk8 = ObjAngle_MoveToward16(w->unk8, t, spd);
        if (w->unk8 == (u16)t)
            w->unk1F4 ^= 2;
    } else {
        t = base - 0x2000;
        w->unk8 = ObjAngle_MoveToward16(w->unk8, t, spd);
        if (w->unk8 == (u16)t)
            w->unk1F4 ^= 2;
    }
}
