/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_ov000_022f53d0();
s32 func_02067d58();

void func_ov000_022f5444(void) {
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x54) & 0xc) != 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_ov000_022f53d0(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x20c), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x340));
    func_ov000_022f53d0(t0, (t0 + 0x238), (t0 + 0x3d0));
    func_ov000_022f53d0(t0, (t0 + 0x264), (t0 + 0x3d0));
    func_ov000_022f53d0(t0, (t0 + 0x290), (t0 + 0x3d0));
    func_ov000_022f53d0(t0, (t0 + 0x2bc), (t0 + 0x400));
    func_ov000_022f53d0(t0, (t0 + 0x2e8), (t0 + 0x400));
    func_ov000_022f53d0(t0, (t0 + 0x314), (t0 + 0x400));
    if (*(u32 *)((u8 *)t0 + 0x1b8) == 0) {
        return;
    }
    func_02067d58((t0 + 0x1b8));
}
