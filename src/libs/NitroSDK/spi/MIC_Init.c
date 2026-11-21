#include <nitro/types.h>

typedef struct {
    u32 f0;
    u32 f4;
} UnkWork_0200dda8;

typedef struct {
    u16 f0;
    u8 pad2[6];
} UnkEntry_0200dda8;

extern u16 data_0207f064;
extern UnkWork_0200dda8 data_0207f068;
extern UnkEntry_0200dda8 data_0207f0c0[4];
extern u8 data_0207f0ac[];
extern u32 data_0207f094;

extern void PXI_Init(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void PXI_SetFifoRecvCallback(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void MicCommonCallback(u32 tag, u32 data, BOOL err);
extern void OS_InitMutex(void *p);

void MIC_Init(void)
{
    if (data_0207f064 != 0) {
        return;
    }
    data_0207f064 = 1;
    data_0207f068.f0 = 0;
    data_0207f068.f4 = 0;
    PXI_Init();
    while (!PXI_IsCallbackReady(9, 1)) {
    }
    *(u32 *)0x027fff90 = 0;
    PXI_SetFifoRecvCallback(9, MicCommonCallback);
}
