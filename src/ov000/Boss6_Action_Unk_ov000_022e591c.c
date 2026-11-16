#include <nitro/types.h>

typedef struct UnkStruct_022e591c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x9f0 - 0x6c];
    u8 unk9F0[0x1118 - 0x9f0];
    s32 unk1118;
} UnkStruct_022e591c;

extern UnkStruct_022e591c *gBoss6;

void Boss6_Unk_ov000_022e620c(void);
void Boss6_State_Unk_ov000_022e552c(void);
void NNS_G3dRenderObjSetCallBack(void *p, void (*f)(void), s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

void Boss6_Action_Unk_ov000_022e591c(void) {
    UnkStruct_022e591c *p = gBoss6;
    NNS_G3dRenderObjSetCallBack(p->unk9F0, Boss6_Unk_ov000_022e620c, 0, 6, 3);
    p->unk1118 = 0;
    p->func = Boss6_State_Unk_ov000_022e552c;
    Sound_PlayVoice(0, 0, 0xd1);
}
