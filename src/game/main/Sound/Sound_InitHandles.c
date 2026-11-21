#include <nitro/types.h>

extern u32 data_02250468[];
extern u32 data_02250460;
extern u8 data_0225045c[];
void NNS_SndHandleInit(void *p);
void MI_CpuFill8(void *dest, u8 data, u32 size);

void Sound_InitHandles(void) {
    u32 i;
    for (i = 0; i < 0x20; i++) {
        NNS_SndHandleInit(&data_02250468[i]);
    }
    NNS_SndHandleInit(&data_02250460);
    MI_CpuFill8(data_0225045c, 0, 4);
}
