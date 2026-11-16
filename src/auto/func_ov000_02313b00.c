/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0204b7ec();
s32 func_ov000_02313e54();
s32 func_ov000_02351d28();

void func_ov000_02313b00(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    if (*(u8 *)t1 != 1) {
        return;
    }
    func_0204b7ec(0, 0, 0x6d);
    func_ov000_02313e54(t0, t1);
    if ((*(u16 *)((u8 *)*(u32 *)((u8 *)t0 + 0x1e0) + 0x4) & 1) != 0) {
        func_ov000_02351d28(t1, t0, 0xc, 0);
        return;
    }
    func_ov000_02351d28(t1, t0, -12, 0);
}
