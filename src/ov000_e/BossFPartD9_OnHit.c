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

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
    u8 pad32A[0x3B2 - 0x32A];
    s16 unk3B2;
} UnkStruct_Player;

void func_ov000_0234aa80(UnkStruct_Player *p);
void Player_OnHurt(UnkStruct_Hit *a, UnkStruct_Hit *b);

void BossFPartD9_OnHit(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_Player *p;
    UnkStruct_StageObj *w;
    s16 v;

    p = b->unk18;
    w = a->unk18;
    if (p->base.unk0 != 1)
        return;
    if (p->unk328 == 0x30) {
        w->unk54 |= 8;
        func_ov000_0234aa80(p);
        w->coll0.unkA &= ~4;
        return;
    }
    if (p->unk328 == 0x31) {
        if (w->unk10 < p->base.unk10) {
            p->base.unk36 = ((a->unk0 + a->unk10 - b->unkC) << 8) - p->base.unk10;
            v = 0x300;
        } else {
            p->base.unk36 = ((a->unk0 + a->unkC - b->unk10) << 8) - p->base.unk10;
            v = -0x300;
        }
        p->base.unk58 |= 4;
        p->base.unk2E = v;
        p->base.unk34 = v;
        return;
    }
    if (p->unk3B2 == 0)
        Player_OnHurt(b, a);
}
