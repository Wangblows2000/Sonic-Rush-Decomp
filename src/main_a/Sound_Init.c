#include <nitro/types.h>

void func_0201b3d4(void);
u32 func_0201db30(void *p, u32 size);
void func_0201bf34(void *p);
void Sound_InitHandles(void);
extern u8 gSoundHeapBuffer[];
extern u32 gSoundHeap;
extern u32 data_02250464;
extern u32 gSESoundHandle;
extern u32 data_02250450;
extern u8 gBGMVolume;
extern u8 gSEVolume;
extern u8 data_02250448;

void Sound_Init(void) {
    func_0201b3d4();
    gSoundHeap = func_0201db30(gSoundHeapBuffer, 0x64000);
    func_0201bf34(&data_02250464);
    func_0201bf34(&gSESoundHandle);
    func_0201bf34(&data_02250450);
    Sound_InitHandles();
    gBGMVolume = 0x7F;
    gSEVolume = 0x7F;
    data_02250448 = 0x7F;
}
