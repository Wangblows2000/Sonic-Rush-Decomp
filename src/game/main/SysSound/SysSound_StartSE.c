#include <nitro/types.h>

typedef struct { u8 pad0[0x14]; void *snd[5]; } UnkWork_02051a74;
extern UnkWork_02051a74 *gSysSound;
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_PlaySeqArc(void *a, s32 b, s32 c);

#pragma thumb on
void SysSound_StartSE(s32 n) {
    s32 a;
    s32 b;

    if (gGameState.unk0 == 0)
        a = 2;
    else
        a = 3;
    switch (n) {
    case 0:
        a = 1;
        b = 0;
        break;
    case 1:
        a = 1;
        b = 1;
        break;
    case 2:
        a = 1;
        b = 2;
        break;
    case 3:
        a = 1;
        b = 3;
        break;
    case 4:
        a = 1;
        b = 5;
        break;
    case 5:
        a = 1;
        b = 0xb;
        break;
    case 6:
        a = 1;
        b = 4;
        break;
    case 7:
        a = 0x11;
        b = 0;
        break;
    case 8:
        a = 0x11;
        b = 1;
        break;
    case 9:
        a = 0x11;
        b = 2;
        break;
    case 10:
        b = 0;
        break;
    case 11:
        b = 1;
        break;
    case 12:
        b = 2;
        break;
    case 13:
        b = 3;
        break;
    case 14:
        b = 4;
        break;
    case 15:
        b = 5;
        break;
    case 16:
        b = 6;
        break;
    case 17:
        b = 7;
        break;
    case 18:
        b = 8;
        break;
    case 19:
        b = 0x10;
        break;
    case 20:
        a = 0xa;
        b = 0;
        break;
    case 21:
        a = 0xa;
        b = 1;
        break;
    case 22:
        a = 0xa;
        b = 2;
        break;
    default:
        return;
    }
    if (gSysSound->snd[2] != NULL) {
        NNS_SndPlayerStopSeq(&gSysSound->snd[2], 0);
        NNS_SndHandleReleaseSeq(&gSysSound->snd[2]);
    }
    Sound_PlaySeqArc(&gSysSound->snd[2], a, b);
}
#pragma thumb reset
