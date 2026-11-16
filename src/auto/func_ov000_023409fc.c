/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
s32 func_ov000_0233ea38();
s32 func_ov000_0233e960();

void func_ov000_023409fc(s32 p0) {
    if ((*(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) & 1) != 0) {
        func_ov000_0233ea38(p0, 0);
        func_ov000_0233e960(p0);
        return;
    }
    func_ov000_0233ea38(p0, 0);
    func_ov000_0233ea38(p0, 1);
}
