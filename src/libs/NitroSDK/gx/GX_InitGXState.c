/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207cfe8[];

void GX_InitGXState(void) {
    *(u16 *)data_0207cfe8 = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x2) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x4) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x6) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x8) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0xa) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0xc) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0xe) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x10) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x12) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x14) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x16) = 0;
    *(u16 *)((u8 *)data_0207cfe8 + 0x18) = 0;
    *(volatile u32 *)0x4000240 = 0;
    *(volatile u8 *)0x4000244 = 0;
    *(volatile u8 *)0x4000245 = 0;
    *(volatile u8 *)0x4000246 = 0;
    *(volatile u16 *)0x4000248 = 0;
}
