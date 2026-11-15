#include <nitro/types.h>

typedef struct { u16 v[8]; } UnkMosaic_02060ab0;
extern UnkMosaic_02060ab0 data_0208922c;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002ab0(s32 a, s32 b, s32 c);
BOOL func_02057350(void);

#pragma thumb on
void func_02060ab0(void) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, (void *)0x06002000, 0xa000);
    func_02002ab0(1, 5, 1);
    *(vu32 *)0x04000000 &= ~0x38000000;
    *(vu32 *)0x04000000 &= ~0x07000000;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & 0x43) | 0x4604;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x4408;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 0;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 1;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & ~3) | 2;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 3;
    if (func_02057350())
        *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x1f00;
    else
        *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x1700;
    data_0208922c.v[0] = data_0208922c.v[1] = data_0208922c.v[2] = data_0208922c.v[3] = data_0208922c.v[4] = data_0208922c.v[5] = 0;
}
#pragma thumb reset
