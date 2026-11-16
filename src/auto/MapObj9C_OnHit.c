/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjRect_PointCheck();
s32 func_ov000_02350adc();

void MapObj9C_OnHit(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    if (*(u8 *)t1 != 1) {
        return;
    }
    s32 t2 = ObjRect_PointCheck(p0, ((s32)*(u32 *)((u8 *)t1 + 0x10) >> 8), ((s32)*(u32 *)((u8 *)t1 + 0x14) >> 8));
    if (t2 == 0) {
        return;
    }
    func_ov000_02350adc(t1, t0);
}
