/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02035704();

void func_ov000_0233c878(s32 p0) {
    func_02035704(p0, 0xf0, 1, 0x4000);
    func_02035704(p0, 0xf0, 1, 0x6000);
    func_02035704(p0, 0xf0, 3, 0x4000);
    func_02035704(p0, 0xf0, 3, 0x6000);
}
