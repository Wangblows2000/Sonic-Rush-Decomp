/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov001_022f1d54();
extern u8 data_ov001_02316840[];
extern u8 data_ov001_0231f068[];
s32 func_02031a68();
extern u8 data_ov001_0231f074[];

void func_ov001_022f1ea0(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2) == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18);
    s32 t2 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18) + 0x84);
    *(u32 *)((u8 *)p0 + 0x74) = (u32)func_ov001_022f1d54;
    *(u32 *)((u8 *)p0 + 0xb8) = (*(u32 *)((u8 *)p0 + 0xb8) & ~0x200);
    *(u32 *)((u8 *)p0 + 0x128) = (*(u32 *)((u8 *)p0 + 0x128) | 0x200);
    *(u32 *)((u8 *)t1 + 0xf0) = (*(u32 *)((u8 *)t1 + 0xf0) | 0x200);
    func_02031a68(t0, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)data_ov001_02316840 + 0x50), 0);
    func_02031a68(t0, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)data_ov001_02316840 + 0x52), 0);
    *(u8 *)((u8 *)t0 + 0x10c) = (*(u8 *)((u8 *)t0 + 0x10c) | 4);
    *(u8 *)((u8 *)t0 + 0x110) = (*(u8 *)((u8 *)t0 + 0x110) | 4);
    func_02031a68(t2, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)data_ov001_02316840 + 0x54), 0);
    func_02031a68(t2, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)data_ov001_02316840 + 0x56), 0);
    *(u8 *)((u8 *)t2 + 0x10c) = (*(u8 *)((u8 *)t2 + 0x10c) | 4);
    *(u8 *)((u8 *)t2 + 0x110) = (*(u8 *)((u8 *)t2 + 0x110) | 4);
    *(u32 *)((u8 *)p0 + 0x50) = 0x5000;
}
