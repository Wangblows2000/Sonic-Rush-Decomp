/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
s32 func_ov000_02307e84();
s32 func_ov000_0234237c();
void MapObj8F_State_Unk_ov000_023082b4();

void func_ov000_02308370(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) & 1);
    if (t0 != 0) {
        s32 t1 = func_ov000_02307e84(p0, t0);
        s32 t2 = func_ov000_0234237c(1, t1);
        if (t2 == 0) {
            *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 0x10);
            *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
            return;
        }
        s32 t3 = (*(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) & ~4);
        *(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) = t3;
        *(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) = (t3 | 8);
        *(u32 *)((u8 *)data_ov000_0236aad8 + 0xdc) = ((s32)(*(u32 *)((u8 *)t2 + 0x224) + *(u32 *)((u8 *)t2 + 0x22c)) >> 1);
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)MapObj8F_State_Unk_ov000_023082b4;
}
