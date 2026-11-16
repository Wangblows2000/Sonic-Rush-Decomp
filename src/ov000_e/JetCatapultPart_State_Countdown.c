#include <nitro/types.h>

typedef struct UnkStruct_0230e238 {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x68 - 0x10];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    struct UnkStruct_0230e238 *unk88;
} UnkStruct_0230e238;

void StageObj_SetAnimation(void *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void JetCatapult_State_Countdown(void);

void JetCatapultPart_State_Countdown(UnkStruct_0230e238 *s) {
    UnkStruct_0230e238 *p = s->unk88;
    if (p == NULL) {
        return;
    }
    if (p->unk68 != JetCatapult_State_Countdown) {
        return;
    }
    if (s->unkC > 0xb5 && p->unkC <= 0xb5) {
        StageObj_SetAnimation(s, 2);
        Sound_PlaySeqArcEx(0, 0, 0x6b);
    }
    if (s->unkC > 0x79 && s->unk88->unkC <= 0x79) {
        StageObj_SetAnimation(s, 3);
        Sound_PlaySeqArcEx(0, 0, 0x6b);
    }
    if (s->unkC > 0x3d && s->unk88->unkC <= 0x3d) {
        StageObj_SetAnimation(s, 4);
        Sound_PlaySeqArcEx(0, 0, 0x6b);
    }
    if (s->unkC > 1 && s->unk88->unkC <= 1) {
        StageObj_SetAnimation(s, 5);
        Sound_PlaySeqArcEx(0, 0, 0xeb);
    }
    s->unkC = s->unk88->unkC;
}
