#include <nitro/types.h>

typedef struct UnkStruct_022de224 {
    u8 pad[0x220];
    void (*func)(void);
    s32 unk224;
    u8 pad1[0xbc4 - 0x228];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
} UnkStruct_022de224;

extern s32 gBossStatus[];

void Boss4_SubState_ClashFlash(void);
void Boss4_SetSpinSpeed(UnkStruct_022de224 *s, s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

void Boss4_Action_Clash(UnkStruct_022de224 *s) {
    gBossStatus[1] = 0x15;
    s->func = Boss4_SubState_ClashFlash;
    s->unk224 = 0;
    Boss4_SetSpinSpeed(s, 0x20);
    s->unkBC4 = 0x180;
    s->unkBC8 = 0xd80;
    s->unkBCC = 0x900;
    Sound_PlaySeqArc(0, 0, 0x8d);
}
