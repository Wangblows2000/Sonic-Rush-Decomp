/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov001_022d1238();
s32 StaffRoll_SetPictureState();

void func_ov001_022d12a8(s32 p0) {
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 4);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcc8) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xccc) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcd4) = -4096;
    *(u32 *)((u8 *)p0 + 0xcd8) = -2048;
    *(u32 *)((u8 *)p0 + 0xcec) = -192;
    *(u32 *)((u8 *)p0 + 0xcf0) = -128;
    StaffRoll_SetPictureState(p0, (u32)func_ov001_022d1238, -128, ((u8 *)p0 + 0xc58));
}

#pragma thumb reset
