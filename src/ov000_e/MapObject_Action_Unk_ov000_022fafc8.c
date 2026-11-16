#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022fafc8;

extern s16 data_0206e470[];

void func_ov000_022faf48(UnkStruct_022fafc8 *s, s32 a);
void MapObject_State_Unk_ov000_022faf7c(void);
s32 FX_Atan2(s32 y, s32 x);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);

void MapObject_Action_Unk_ov000_022fafc8(UnkStruct_022fafc8 *s, UnkStruct_StageObj *o) {
    s16 sp;
    u16 a;

    if (s->base.unk1E0->unk2 != 0xb1 && s->base.unk1E0->unk2 != 0xb2)
        func_ov000_022faf48(s, 0);
    s->base.unk5C |= 0x10;
    s->base.unk5C |= 0x100;
    s->base.unk58 |= 0x10;
    s->base.unk58 |= 0x100;
    s->base.unk54 |= 0x10;
    s->base.unk68 = MapObject_State_Unk_ov000_022faf7c;
    s->base.unk34 = 0;
    sp = 0x37f - (Rand() & 0x1fe);
    s->base.unk2E = o->unk10 - o->unk1C;
    s->base.unk30 = o->unk14 - o->unk20;
    a = FX_Atan2(o->unk2A, o->unk28) - 0x4000;
    if (o->unk5C & 1)
        a += 0x8000;
    s->base.unk2E += (s16)((sp * data_0206e470[((a & 0xffff) >> 4) * 2 + 1]) >> 12);
    s->base.unk30 += (s16)((sp * data_0206e470[((a & 0xffff) >> 4) * 2]) >> 12);
    Sound_PlaySeqArcEx(0, 0, 0x7a);
    s->base.unkC = 0x20;
    s->base.unk8 = 0;
    s->base.unk1F4 &= ~5;
}
