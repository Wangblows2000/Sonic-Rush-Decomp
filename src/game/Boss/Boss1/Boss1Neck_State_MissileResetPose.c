#include <nitro/types.h>

typedef struct UnkStruct_022c7514 UnkStruct_022c7514;
struct UnkStruct_022c7514 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022c7514 *s);
    u8 pad1[0x224 - 0x6c];
    s32 unk224;
    u8 unk228[4];
};

void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss1_ResetCamera(void);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void Boss1Neck_State_MissileWaitResetPose(UnkStruct_022c7514 *s);

void Boss1Neck_State_MissileResetPose(UnkStruct_022c7514 *s) {
    Boss1_SetPartAnimation(s->unk228, s->unk224, 5, 0x1000, 0);
    s->unk10 = 0xe100;
    s->unk14 = 0x20000;
    s->unk18 = -0x8e00;
    Boss1_ResetCamera();
    Sound_PlayVoice(0, 0, 0x7e);
    s->func = Boss1Neck_State_MissileWaitResetPose;
}
