#include <nitro/types.h>

typedef struct UnkStruct_022d63c0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c0 - 0x6c];
    void *unk2C0;
} UnkStruct_022d63c0;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Boss3PartCC_State_Defeated(void);

void Boss3PartCC_Action_Defeat(UnkStruct_022d63c0 *s) {
    NNS_SndPlayerStopSeq(s->unk2C0, 0x20);
    NNS_SndHandleReleaseSeq(s->unk2C0);
    s->func = Boss3PartCC_State_Defeated;
}
