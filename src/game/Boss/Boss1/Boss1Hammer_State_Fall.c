#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x68 - 0x32];
    void *unk68;
} UnkStruct_022c6850;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss1Hammer_State_Land(UnkStruct_022c6850 *p);

void Boss1Hammer_State_Fall(UnkStruct_022c6850 *p) {
    p->unk30 += 0x100;
    if (p->unk14 + p->unk30 < 0x1dc00)
        return;
    p->unk14 = 0x1dc00;
    p->unk30 = 0;
    Sound_PlaySeqArc(0, 0, 0x7b);
    p->unk68 = Boss1Hammer_State_Land;
}
