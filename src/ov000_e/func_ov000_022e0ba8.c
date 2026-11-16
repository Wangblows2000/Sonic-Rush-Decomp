#include <ov000_c/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xC - 4];
    s16 unkC;
    u8 padE[2];
    s16 unk10;
    u8 pad12[0x18 - 0x12];
    void *unk18;
} UnkStruct_Hit;

void func_ov000_022e0ba8(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *p;
    UnkStruct_StageObj *w;
    s32 spd;

    p = b->unk18;
    w = a->unk18;
    if (p->unk0 != 1)
        return;
    if (p->unk58 & 0x8000)
        spd = p->unk2E;
    else
        spd = p->unk34;
    if (w->unk10 < p->unk10) {
        if (spd > 0)
            return;
        p->unk36 = ((a->unk0 + a->unk10 - b->unkC) << 8) - p->unk10;
        p->unk58 |= 4;
        p->unk2E = 0;
        p->unk34 = 0;
    } else {
        if (spd < 0)
            return;
        p->unk36 = ((a->unk0 + a->unkC - b->unk10) << 8) - p->unk10;
        p->unk58 |= 4;
        p->unk2E = 0;
        p->unk34 = 0;
    }
}
