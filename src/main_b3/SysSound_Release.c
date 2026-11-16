#include <nitro/types.h>

typedef struct { u8 pad0[0xc]; void *task0; void *task1; void *snd[5]; } UnkWork_020521b4;
extern UnkWork_020521b4 *gSysSound;
extern void *gMainHeap;
extern u8 gBGMVolume;
extern u8 gSEVolume;
extern u8 data_02250448;

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
static inline void Free_020521b4(u8 *blk) {
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
}

void SysSound_Release(void) {
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
        Sound_ResetHeap();
        if (gSysSound->task0 != NULL) {
            Task_Destroy(gSysSound->task0);
            gSysSound->task0 = NULL;
        }
        if (gSysSound->task1 != NULL) {
            Task_Destroy(gSysSound->task1);
            gSysSound->task1 = NULL;
        }
        Free_020521b4((u8 *)gSysSound);
        gSysSound = NULL;
        gBGMVolume = 0x7f;
        gSEVolume = 0x7f;
        data_02250448 = 0x7f;
    }
}
#pragma thumb reset
