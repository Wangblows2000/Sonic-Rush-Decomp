#include <nitro/types.h>

void NNS_SndHeapClear(u32 p);
void func_0201db14(u32 p);
u32 NNS_SndHeapCreate(void *p, u32 size);
void NNS_SndHandleInit(void *p);
extern u8 gSoundHeapBuffer[];
extern u32 gSoundHeap;
extern u32 data_02250464;
extern u32 gSESoundHandle;
extern u32 data_02250450;
extern u8 gBGMVolume;
extern u8 gSEVolume;
extern u8 data_02250448;

void Sound_ResetHeap(void) {
    NNS_SndHeapClear(gSoundHeap);
    func_0201db14(gSoundHeap);
    gSoundHeap = NNS_SndHeapCreate(gSoundHeapBuffer, 0x64000);
    NNS_SndHandleInit(&data_02250464);
    NNS_SndHandleInit(&gSESoundHandle);
    NNS_SndHandleInit(&data_02250450);
    gBGMVolume = 0x7F;
    gSEVolume = 0x7F;
    data_02250448 = 0x7F;
}
