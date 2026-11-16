/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_023070fc();
s32 func_02036754();
s32 func_ov001_0230c0a4();

void func_ov001_0230ea24(u8 *p0) {
    func_ov001_023070fc(1, *(u32 *)((u8 *)p0 + 0x20), *(u32 *)((u8 *)p0 + 0x24), *(u32 *)((u8 *)p0 + 0x28));
    func_02036754(0, 0x10, 0x11);
    s32 t1 = func_ov001_0230c0a4();
    *(u32 *)((u8 *)t1 + 0x148) = (*(u32 *)((u8 *)t1 + 0x148) + *(u16 *)((u8 *)p0 + 0x5e));
}
