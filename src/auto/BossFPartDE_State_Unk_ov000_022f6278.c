/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f685c();
void func_ov000_022f61a0();

void BossFPartDE_State_Unk_ov000_022f6278(u8 *p0) {
    func_ov000_022f685c();
    *(u16 *)((u8 *)p0 + 0x210) = (*(u16 *)((u8 *)p0 + 0x210) + 0x800);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x4ae) + 0x1);
    s32 t1 = *(u16 *)((u8 *)p0 + 0x4ae);
    *(u16 *)((u8 *)p0 + 0x4ae) = t0;
    if ((u32)t1 > (u32)0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f61a0;
    }
}
