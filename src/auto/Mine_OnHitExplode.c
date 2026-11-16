/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Mine_Explode();

void Mine_OnHitExplode(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    Mine_Explode(t0, t1);
}
