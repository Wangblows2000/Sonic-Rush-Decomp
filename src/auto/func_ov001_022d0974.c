/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d0220();
s32 func_02033550();
void func_ov001_022d0930();
s32 func_ov001_022d1404();

void func_ov001_022d0974(s32 p0) {
    func_ov001_022d0220();
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) | 4);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcbc) = 0xa0000;
    *(u32 *)((u8 *)p0 + 0xcc8) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xd04) = 1;
    func_02033550(((u8 *)p0 + 0xc2c), 0x21, 0x80, ((u8 *)p0 + 0xca0));
    func_ov001_022d1404(p0, (u32)func_ov001_022d0930);
}

#pragma thumb reset
