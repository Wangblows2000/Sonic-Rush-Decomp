/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0230d6e0();
extern u8 data_ov001_0231f694[];
s32 func_02031bd0();
s32 func_02031a68();
void func_ov001_02311130();
s32 func_ov001_02311098();

void func_ov001_0231120c(u8 *p0) {
    s32 t0 = func_ov001_0230d6e0();
    if (t0 < 0x2400) {
        *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & ~4);
        s32 t1 = *(u32 *)((u8 *)p0 + 0xac);
        if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & 2) == 0) {
            return;
        }
        func_02031bd0(t1, *(u32 *)((u8 *)*(u32 *)data_ov001_0231f694 + 0x13c), 0);
        func_02031a68(*(u32 *)((u8 *)p0 + 0xac), 0, *(u32 *)((u8 *)*(u32 *)data_ov001_0231f694 + 0x140), 3, 0);
        func_02031a68((*(u32 *)data_ov001_0231f694 + 0x4), 4, *(u32 *)((u8 *)*(u32 *)data_ov001_0231f694 + 0x144), 1, 0);
        *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) | 4);
        *(u32 *)((u8 *)p0 + 0x22c) = (u32)func_ov001_02311130;
        return;
    }
    s32 t2 = func_ov001_02311098(t0);
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x114) = t2;
}
