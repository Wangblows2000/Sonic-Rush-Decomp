/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_DisableInterrupts();
extern u8 data_0207f0e4[];
s32 OS_RestoreInterrupts();
s32 RTCi_ReadRawTimeAsync();

s32 func_0200f0e4(s32 p0, s32 p1, s32 p2) {
    s32 t0 = OS_DisableInterrupts();
    s32 t1 = *(u32 *)data_0207f0e4;
    if (t1 != 0) {
        OS_RestoreInterrupts(t0, data_0207f0e4, t1);
        return 1;
    }
    *(u32 *)data_0207f0e4 = 1;
    OS_RestoreInterrupts(t0, data_0207f0e4, 1);
    *(u32 *)((u8 *)data_0207f0e4 + 0x14) = 1;
    *(u32 *)((u8 *)data_0207f0e4 + 0x18) = 0;
    *(u32 *)((u8 *)data_0207f0e4 + 0x8) = p0;
    *(u32 *)((u8 *)data_0207f0e4 + 0x4) = p1;
    *(u32 *)((u8 *)data_0207f0e4 + 0x10) = p2;
    s32 t2 = RTCi_ReadRawTimeAsync(data_0207f0e4, 0, 1);
    return ((t2 != 0) ? 0 : 3);
}
