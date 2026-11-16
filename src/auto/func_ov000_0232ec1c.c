/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0201c070();
s32 func_0201bf1c();
s32 func_020371c8();

void func_ov000_0232ec1c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    *(u16 *)((u8 *)t0 + 0x4) = (*(s16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x4) - 0x1);
    if (*(s16 *)((u8 *)t0 + 0x4) >= 0) {
        return;
    }
    func_0201c070(*(u32 *)t0, 0x20);
    func_0201bf1c(*(u32 *)t0);
    func_020371c8(*(u32 *)data_022b4574);
}
