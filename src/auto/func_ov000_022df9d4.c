/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b4c[];
extern u8 data_ov000_02369b50[];
extern u8 data_ov000_023658e4[];
extern u8 data_ov000_0236377c[];
s32 func_ov000_022c5d4c();
extern u8 data_ov000_023657e8[];
extern u8 data_ov000_023636e8[];
extern u8 data_ov000_023657ec[];
extern u8 data_ov000_023636f0[];

void func_ov000_022df9d4(s32 p0) {
    if (((s32)p0 & 3) != 0) {
        return;
    }
    if (((s32)p0 & 4) != 0) {
        func_ov000_022c5d4c(*(u32 *)((u8 *)data_ov000_02369b50 + 0x18), data_ov000_023658e4, *(u32 *)((u8 *)(*(u32 *)data_ov000_02369b4c + 0x1000) + 0xf4), data_ov000_0236377c, 8);
        func_ov000_022c5d4c(*(u32 *)((u8 *)data_ov000_02369b50 + 0x1c), data_ov000_023657e8, *(u32 *)((u8 *)(*(u32 *)data_ov000_02369b4c + 0x1000) + 0xfc), data_ov000_023636e8, 1);
        func_ov000_022c5d4c(*(u32 *)((u8 *)data_ov000_02369b50 + 0x20), data_ov000_023657ec, *(u32 *)((u8 *)(*(u32 *)data_ov000_02369b4c + 0x1000) + 0x104), data_ov000_023636f0, 1);
        return;
    }
    func_ov000_022c5d4c(*(u32 *)((u8 *)data_ov000_02369b50 + 0x18), data_ov000_023658e4, *(u32 *)((u8 *)(*(u32 *)data_ov000_02369b4c + 0x1000) + 0xf8), data_ov000_0236377c, 8);
    func_ov000_022c5d4c(*(u32 *)((u8 *)data_ov000_02369b50 + 0x1c), data_ov000_023657e8, *(u32 *)((u8 *)(*(u32 *)data_ov000_02369b4c + 0x1000) + 0x100), data_ov000_023636e8, 1);
    func_ov000_022c5d4c(*(u32 *)((u8 *)data_ov000_02369b50 + 0x20), data_ov000_023657ec, *(u32 *)((u8 *)(*(u32 *)data_ov000_02369b4c + 0x1000) + 0x108), data_ov000_023636f0, 1);
}
