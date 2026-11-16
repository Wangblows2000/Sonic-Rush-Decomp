/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022e5684();
s32 func_ov001_022e5618();
s32 func_ov001_022e4eb8();
s32 func_ov001_022e2ad8();
s32 func_ov001_022e3bec();
s32 func_ov001_022e36fc();

void func_ov001_022e870c(s32 p0) {
    func_ov001_022e5684(((u8 *)p0 + 0x4), *(u32 *)((u8 *)p0 + 0x1cd4));
    func_ov001_022e5618(((u8 *)p0 + 0x4), 1);
    func_ov001_022e4eb8(((u8 *)p0 + 0x674), ((u8 *)p0 + 0x4), *(u32 *)((u8 *)p0 + 0x1ce0), *(u32 *)((u8 *)p0 + 0x1cd8));
    s32 t0 = func_ov001_022e2ad8();
    func_ov001_022e3bec(((u8 *)p0 + 0x1994), t0, *(u32 *)((u8 *)p0 + 0x1ce8));
    func_ov001_022e36fc(((u8 *)p0 + 0x1b08), *(u32 *)((u8 *)p0 + 0x1cdc), *(u32 *)((u8 *)p0 + 0x1ce4), *(u32 *)((u8 *)p0 + 0x1cdc));
}

#pragma thumb reset
