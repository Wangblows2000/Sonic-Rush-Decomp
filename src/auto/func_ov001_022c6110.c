/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
extern u8 data_022c4678[];
s32 func_02057364();
void func_ov001_022c60e0();
s32 ExtraEnding_SetState();

void func_ov001_022c6110(s32 p0) {
    *(u16 *)((u8 *)gDisplayMain + 0x48) = 0x10;
    *(u16 *)((u8 *)gDisplaySub + 0x48) = 0x10;
    *(u16 *)data_022c4678 = 0x2a;
    func_02057364(p0, data_022c4678, gDisplaySub, gDisplayMain);
    ExtraEnding_SetState(p0, (u32)func_ov001_022c60e0);
}
