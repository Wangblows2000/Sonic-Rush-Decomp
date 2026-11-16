/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov001_022f4e88();
extern u8 data_ov001_0231f068[];
s32 func_02031a68();

void func_ov001_022f4f54(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x74) = (u32)func_ov001_022f4e88;
    func_02031a68(t0, 0, *(u32 *)data_ov001_0231f068, 0x14, 0);
}
