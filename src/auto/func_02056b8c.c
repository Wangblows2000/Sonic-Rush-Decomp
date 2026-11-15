/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 MIi_CpuCopy16();
s32 func_02035704();
s32 func_02056af0();

void func_02056b8c(s32 p0, u8 *p1) {
    s32 t0 = (((s32)p1 + *(u32 *)((u8 *)p1 + 0x10)) + 0x1e4);
    MIi_CpuCopy16((((s32)p1 + *(u32 *)((u8 *)p1 + 0x10)) + 0x1e4), ((u8 *)p0 + 0x14), 0x20);
    MIi_CpuCopy16(t0, ((u8 *)p0 + 0x34), 0x20);
    func_02035704(t0, 0x10, 0, 0x50001e0);
    s32 t1 = func_02056af0(p1);
}

#pragma thumb reset
