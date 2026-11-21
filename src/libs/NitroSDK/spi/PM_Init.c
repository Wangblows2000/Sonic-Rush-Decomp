#include <nitro/types.h>

typedef struct {
    u32 f0;
    u32 f4;
} UnkWork_0200e8dc;

typedef struct {
    u16 f0;
    u8 pad2[6];
} UnkEntry_0200e8dc;

extern u16 data_0207f084;
extern UnkWork_0200e8dc data_0207f09c;
extern UnkEntry_0200e8dc data_0207f0c0[4];
extern u8 data_0207f0ac[];
extern u32 data_0207f094;

extern void PXI_Init(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void PXI_SetFifoRecvCallback(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void PMi_CommonCallback(u32 tag, u32 data, BOOL err);
extern void OS_InitMutex(void *p);

void PM_Init(void)
{
    s32 i;
    if (data_0207f084 != 0) {
        return;
    }
    data_0207f084 = 1;
    data_0207f09c.f0 = 0;
    data_0207f09c.f4 = 0;
    PXI_Init();
    while (!PXI_IsCallbackReady(8, 1)) {
    }
    PXI_SetFifoRecvCallback(8, PMi_CommonCallback);
    for (i = 0; i < 4; i++) {
        data_0207f0c0[i].f0 = 0;
    }
    OS_InitMutex(data_0207f0ac);
    data_0207f094 = *(u32 *)0x027ffc3c;
}
