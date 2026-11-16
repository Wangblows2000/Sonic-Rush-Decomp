/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023425f4();

void MapObj75_State_Unk_ov000_023034f8(u8 *p0) {
    func_ov000_023425f4(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    s32 t0 = *(u32 *)((u8 *)p0 + 0x234);
    if (t0 != 0) {
        ((s32 (*)())t0)(p0, 0, t0);
    }
    if (*(u32 *)((u8 *)p0 + 0x238) == 0) {
        return;
    }
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x238))(p0, 1, *(u32 *)((u8 *)p0 + 0x238));
}
