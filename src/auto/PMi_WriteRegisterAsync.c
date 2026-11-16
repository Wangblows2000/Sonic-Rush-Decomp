/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 PMi_Lock();
extern u8 data_0207f09c[];
s32 PMi_SendPxiData();

s32 PMi_WriteRegisterAsync(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = PMi_Lock();
    if (t0 == 0) {
        return 1;
    }
    *(u32 *)((u8 *)data_0207f09c + 0x4) = p2;
    *(u32 *)((u8 *)data_0207f09c + 0x8) = p3;
    PMi_SendPxiData((((s32)p0 & 0xff) | 0x2006400));
    PMi_SendPxiData((((s32)p1 & 0xffff) | 0x1010000));
    return 0;
}
