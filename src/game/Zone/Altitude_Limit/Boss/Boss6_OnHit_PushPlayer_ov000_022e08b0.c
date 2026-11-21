#include <game/Stage/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xC - 4];
    s16 unkC;
    u8 padE[2];
    s16 unk10;
    u8 pad12[0x18 - 0x12];
    void *unk18;
} UnkStruct_Hit;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xD5E - 0x20C];
    u16 unkD5E;
    u8 padD60[0xF4C - 0xD60];
    s16 unkF4C;
} UnkStruct_Boss;

void Boss6_OnHit_PushPlayer_ov000_022e08b0(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_Boss *w;
    UnkStruct_StageObj *p;
    p = b->unk18;
    w = a->unk18;
    s32 spd;

    if (p->unk0 != 1)
        return;
    if (p->unk58 & 0x8000)
        spd = p->unk2E;
    else
        spd = p->unk34;
    if ((w->unkF4C != 0 && w->unkD5E == 0) || (w->unkF4C == 0 && w->unkD5E != 0))
        p->unk36 = ((a->unk0 - a->unkC - b->unkC + 1) << 8) - p->unk10;
    else
        p->unk36 = ((a->unk0 + a->unkC - b->unk10 - 1) << 8) - p->unk10;
    if (spd == 0)
        return;
    p->unk58 |= 4;
    p->unk34 = 0;
    p->unk2E = p->unk34;
}
