/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_0232cbf4();
void func_ov000_0232d3dc();
s32 func_0206d190();
s32 func_02033550();

void func_ov000_0232cc2c(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x276) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0232cbf4;
    *(u32 *)((u8 *)p0 + 0x70) = (u32)func_ov000_0232d3dc;
    *(u32 *)((u8 *)p0 + 0x168) = *(u32 *)((u8 *)p0 + 0xc8);
    *(u32 *)((u8 *)p0 + 0x1b0) = *(u32 *)((u8 *)p0 + 0x110);
    *(u32 *)((u8 *)p0 + 0x1f8) = (*(u32 *)((u8 *)p0 + 0xc8) + 0x400);
    *(u32 *)((u8 *)p0 + 0x240) = (*(u32 *)((u8 *)p0 + 0x110) + 0x400);
    func_0206d190(*(u32 *)*(u32 *)((u8 *)p0 + 0x8c), 0xb, (*(u16 *)((u8 *)p0 + 0xd8) & 0xff), 0);
    func_0206d190(*(u32 *)*(u32 *)((u8 *)p0 + 0x8c), 0xb, (*(u16 *)((u8 *)p0 + 0xd8) & 0xff), 1);
    func_02033550(((u8 *)p0 + 0x134), 0xb);
    func_02033550(((u8 *)p0 + 0x17c), 0xb);
    func_02033550(((u8 *)p0 + 0x1c4), 0xc);
    func_02033550(((u8 *)p0 + 0x20c), 0xc);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 0x3000);
}
