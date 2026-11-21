#include <nitro/types.h>

typedef struct UnkStruct_022edbec {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x230 - 0x6c];
    u16 unk230;
} UnkStruct_022edbec;

extern u8 gBGMSoundHandle;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void Boss7_State_ClashWaitDelay(void);

void Boss7_State_ClashFadeOutBGM(UnkStruct_022edbec *s) {
    s->unk230 = 0;
    NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0x3c);
    s->func = Boss7_State_ClashWaitDelay;
}
