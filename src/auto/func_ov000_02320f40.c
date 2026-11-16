/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_02320ce0();
extern u8 gGameState[];

void func_ov000_02320f40(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02320ce0;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 2);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 0x200);
    *(u32 *)((u8 *)p0 + 0x8) = ((((u32)(((*(u8 *)((u8 *)p0 + 0x3) + (*(u32 *)((u8 *)gGameState + 0x20) + *(u8 *)((u8 *)p0 + 0x2))) << 0x10) * 0x196225 + 0x3c6ef35f) >> 0x10) & 0xff) + 0x80);
}
