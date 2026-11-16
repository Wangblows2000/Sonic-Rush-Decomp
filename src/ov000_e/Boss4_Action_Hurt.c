#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x220];
    void *unk220;
    s32 unk224;
    u8 pad228[0xBC4 - 0x228];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
} UnkStruct_022de018;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;
extern s32 data_ov000_02363538[];

void func_ov000_022db900(UnkStruct_022de018 *s, s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void func_ov000_022ddedc(void);

void Boss4_Action_Hurt(UnkStruct_022de018 *s) {
    gBossStatus.unk4 = 0x16;
    s->unk220 = func_ov000_022ddedc;
    s->unk224 = 0;
    func_ov000_022db900(s, 0x20);
    s->unkBC4 = 0x180;
    s->unkBC8 = -0x600;
    s->unkBCC = -0xa00;
    Sound_PlaySeqArc(0, 0, 0x7b);
    Sound_PlayVoice(0, 0, data_ov000_02363538[Rand() & 1]);
}
