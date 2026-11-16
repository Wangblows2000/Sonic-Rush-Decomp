#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; s32 unk8; void *task0; void *task1; void *snd[5]; s32 unk28; s32 unk2C; u8 pad30[0x10]; } UnkWork_020522f4;
extern UnkWork_020522f4 *gSysSound;
extern void *gMainHeap;
extern u8 gBGMVolume;
extern u8 gSEVolume;
extern u8 data_02250448;
extern u8 data_0207b02c[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void NNS_SndHandleInit(void *p);
void func_02036854(void *p);
void NNS_SndPlayerSetVolume(void *p, u8 v);

#pragma thumb on
static inline u8 *Alloc_020522f4(u32 size) {
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void SysSound_Init(void) {
    if (gSysSound == NULL) {
        gSysSound = (UnkWork_020522f4 *)Alloc_020522f4(0x60);
        gSysSound->unk0 = 5;
        gSysSound->unk4 = 7;
        gSysSound->unk8 = 7;
        gSysSound->task0 = NULL;
        gSysSound->task1 = NULL;
        NNS_SndHandleInit(&gSysSound->snd[0]);
        NNS_SndHandleInit(&gSysSound->snd[1]);
        NNS_SndHandleInit(&gSysSound->snd[2]);
        NNS_SndHandleInit(&gSysSound->snd[3]);
        NNS_SndHandleInit(&gSysSound->snd[4]);
        gSysSound->unk28 = 0;
        gSysSound->unk2C = 0;
        func_02036854(data_0207b02c);
        gBGMVolume = 0x7f;
        gSEVolume = 0x7f;
        data_02250448 = 0x7f;
        NNS_SndPlayerSetVolume(&gSysSound->snd[0], 0x7f);
        NNS_SndPlayerSetVolume(&gSysSound->snd[1], gBGMVolume);
        NNS_SndPlayerSetVolume(&gSysSound->snd[2], gSEVolume);
        NNS_SndPlayerSetVolume(&gSysSound->snd[3], data_02250448);
        NNS_SndPlayerSetVolume(&gSysSound->snd[4], data_02250448);
    }
}
#pragma thumb reset
