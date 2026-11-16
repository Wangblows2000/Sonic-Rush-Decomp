#include <nitro/types.h>

typedef struct {
    u8 pad0[0x220];
    void *unk220;
    s32 unk224;
    u8 pad228[0xbec - 0x228];
    s32 unkBEC;
} UnkStruct_022ddbec;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss4_SubState_Unk_ov000_022ddb44(UnkStruct_022ddbec *p);

void Boss4_SubState_Unk_ov000_022ddbec(UnkStruct_022ddbec *p) {
    s32 lim;

    if (p->unkBEC != 0)
        lim = 0;
    else
        lim = 0x1e;
    if (p->unk224 <= lim)
        return;
    p->unk220 = Boss4_SubState_Unk_ov000_022ddb44;
    p->unk224 = 0;
    Sound_PlaySeqArc(0, 0, 0x8b);
}
