#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x454 - 0x6c];
    u16 unk454;
} UnkStruct_022f0b44;

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
} UnkStruct_022f0b44_g;

extern UnkStruct_022f0b44_g *gBossF;
extern s32 gGameState;
extern u16 gTalkDemoId;

void Sound_PlaySeq(s32 a, s32 b);
void TalkDemo_Create(void);
void BossFPartD8_State_SkyChargeStartRetreat(UnkStruct_022f0b44 *p);

void BossFPartD8_State_WaitTalkDemo(UnkStruct_022f0b44 *p) {
    if (p->unk454++ <= 0x78)
        return;
    Sound_PlaySeq(0, 0x13);
    gBossF->unk20C |= 1;
    if (gGameState == 0)
        gTalkDemoId = 0xf;
    else
        gTalkDemoId = 0x10;
    TalkDemo_Create();
    p->unk68 = BossFPartD8_State_SkyChargeStartRetreat;
}
