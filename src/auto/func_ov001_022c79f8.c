/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void func_ov001_022c79d4();
s32 func_ov001_022c7a34();

void func_ov001_022c79f8(s32 p0) {
    *(u16 *)((u8 *)gDisplayMain + 0x48) = 0x10;
    *(u16 *)((u8 *)gDisplaySub + 0x48) = 0x10;
    func_ov001_022c7a34(p0, (u32)func_ov001_022c79d4, gDisplaySub, gDisplayMain);
}
