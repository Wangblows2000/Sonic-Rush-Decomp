#include <nitro/types.h>

typedef struct { u8 pad0[4]; s32 unk4; u8 pad8[4]; void *task0; void *task1; void *snd[5]; } UnkWork_02052000;
extern UnkWork_02052000 *gSysSound;
extern void *gMainHeap;
extern u8 gBGMVolume;
extern u8 gSEVolume;
extern u8 gVoiceVolume;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);
void Task_Destroy(void *t);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
void func_02052000(void) {
    if (gSysSound != NULL) {
        if (gSysSound->snd[0] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[0], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[0]);
        }
        if (gSysSound->snd[1] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[1], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[1]);
        }
        if (gSysSound->snd[2] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[2], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[2]);
        }
        if (gSysSound->snd[3] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[3], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[3]);
        }
        if (gSysSound->snd[4] != NULL) {
            NNS_SndPlayerStopSeq(&gSysSound->snd[4], 0);
            NNS_SndHandleReleaseSeq(&gSysSound->snd[4]);
        }
        NNS_SndStopSoundAll();
        gSysSound->unk4 = 7;
    }
}
#pragma thumb reset
