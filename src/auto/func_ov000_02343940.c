/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
extern u8 data_022c4560[];
s32 func_02069090();

void func_ov000_02343940(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    if ((*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x20) != 0) {
        if (*(u8 *)((u8 *)t0 + 0x325) != 0) {
            func_02069090(t0, (t0 + 0x2f8));
            return;
        }
    }
    s32 t1 = (*(u16 *)((u8 *)(t0 + 0x200) + 0xd6) | 0x20);
    *(u16 *)((u8 *)(t0 + 0x200) + 0xd6) = t1;
    func_02069090(t0, (t0 + 0x2cc), (t0 + 0x200), t1);
    func_02069090(t0, (t0 + 0x2a0));
}
