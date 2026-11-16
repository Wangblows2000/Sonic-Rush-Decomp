/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BrightnessFade_Create();
void func_020480ec();
s32 func_0204d5f8();

void Marker_State_Unk_ov000_02310418(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0xc) == 0x10) {
        BrightnessFade_Create(4, 0x100);
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0xc) = (t0 - 0x1);
        return;
    }
    func_0204d5f8((u32)func_020480ec);
}
