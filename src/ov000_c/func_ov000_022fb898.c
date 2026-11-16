#include <ov000_c/stageobj.h>

void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void EffZakoBombBurst_Create(UnkStruct_StageObj *w, s32 a, s32 b);
UnkStruct_StageObj *EffZakoDebris_Create(UnkStruct_StageObj *w, s32 a, s32 b, s32 c, s32 d, u8 e);

void func_ov000_022fb898(UnkStruct_StageObj *w) {
    u8 r;
    UnkStruct_StageObj *o;

    Sound_PlaySeqArcEx(0, 0, 0x33);
    EffZakoBombBurst_Create(w, 0, 0);
    r = Rand() & 3;
    o = EffZakoDebris_Create(w, 0, -0x18, 0, -0x200, r + 4);
    if ((o != NULL && (w->unk58 & 0x1000)) || (w->unk1F4 & 0x40))
        o->unk58 |= 0x100;
    o = EffZakoDebris_Create(w, 0, -0x18, 0, -0x200, ((r + 1) & 3) + 4);
    if ((o != NULL && (w->unk58 & 0x1000)) || (w->unk1F4 & 0x40))
        o->unk58 |= 0x100;
}
