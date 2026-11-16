#include <nitro/types.h>

typedef struct { u8 pad0[0x14]; void *snd[5]; u8 pad28[4]; s32 unk2C; } UnkWork_020519b0;
extern UnkWork_020519b0 *gSysSound;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_PlayVoice(void *a, s32 b, s32 c);

#pragma thumb on
void func_020519b0(s32 n) {
    s32 a;
    s32 b;

    switch (n) {
    case 0:
        a = 1;
        b = 6;
        break;
    case 1:
        a = 1;
        b = 5;
        break;
    case 2:
        a = 1;
        b = 7;
        break;
    case 3:
        a = 1;
        b = 8;
        break;
    case 4:
        a = 1;
        b = 9;
        break;
    case 5:
        a = 1;
        b = 0xa;
        break;
    case 6:
        a = 0x11;
        b = 3;
        break;
    case 7:
        a = 0x11;
        b = 4;
        break;
    default:
        return;
    }
    if (gSysSound->unk2C == 0) {
        if (gSysSound->snd[3] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[3], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[3]);
        }
        Sound_PlayVoice(&gSysSound->snd[3], a, b);
    } else {
        if (gSysSound->snd[4] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[4], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[4]);
        }
        Sound_PlayVoice(&gSysSound->snd[4], a, b);
    }
}
#pragma thumb reset
