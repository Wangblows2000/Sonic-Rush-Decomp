#include <nitro/types.h>

typedef struct { u8 pad0[4]; s32 unk4; u8 pad8[0xc]; void *snd[5]; s32 unk28; } UnkWork_02051b7c;
extern UnkWork_02051b7c *gSysSound;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_PlaySeq(void *a, s32 b);

#pragma thumb on
void func_02051b7c(s32 n) {
    s32 id;

    if (gSysSound->unk4 == n)
        return;
    switch (n) {
    case 0:
        id = 1;
        break;
    case 1:
        id = 2;
        break;
    case 2:
        id = 0x20;
        break;
    case 3:
        id = 0x21;
        break;
    case 4:
        id = 3;
        break;
    case 5:
        id = 4;
        break;
    default:
        return;
    }
    if (gSysSound->unk28 == 0) {
        if (gSysSound->snd[0] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[0], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[0]);
        }
        Sound_PlaySeq(&gSysSound->snd[0], id);
        gSysSound->unk4 = n;
    } else {
        if (gSysSound->snd[1] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[1], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[1]);
        }
        Sound_PlaySeq(&gSysSound->snd[1], id);
    }
}
#pragma thumb reset
