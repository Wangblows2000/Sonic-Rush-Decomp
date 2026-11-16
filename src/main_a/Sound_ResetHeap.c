#include <nitro/types.h>

void NNS_SndHeapClear(u32 p);
void NNS_SndHeapDestroy(u32 p);
u32 NNS_SndHeapCreate(void *p, u32 size);
void NNS_SndHandleInit(void *p);
extern u8 gSoundHeapBuffer[];
extern u32 gSoundHeap;
extern u32 gBGMSoundHandle;
extern u32 gSESoundHandle;
extern u32 gVoiceSoundHandle;
extern u8 gBGMVolume;
extern u8 gSEVolume;
extern u8 gVoiceVolume;

void Sound_ResetHeap(void) {
    NNS_SndHeapClear(gSoundHeap);
    NNS_SndHeapDestroy(gSoundHeap);
    gSoundHeap = NNS_SndHeapCreate(gSoundHeapBuffer, 0x64000);
    NNS_SndHandleInit(&gBGMSoundHandle);
    NNS_SndHandleInit(&gSESoundHandle);
    NNS_SndHandleInit(&gVoiceSoundHandle);
    gBGMVolume = 0x7F;
    gSEVolume = 0x7F;
    gVoiceVolume = 0x7F;
}
