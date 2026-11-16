/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0208922c[];
extern u8 data_02087d04[];
void func_ov001_022c7218();
s32 func_ov001_022c7a34();

void func_ov001_022c723c(s32 p0) {
    *(u16 *)((u8 *)data_0208922c + 0x48) = 0x10;
    *(u16 *)((u8 *)data_02087d04 + 0x48) = 0x10;
    func_ov001_022c7a34(p0, (u32)func_ov001_022c7218, data_02087d04, data_0208922c);
}
