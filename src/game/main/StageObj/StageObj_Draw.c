/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_Draw3D();
s32 StageObj_Draw2D();

void StageObj_Draw(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x124);
    if (t0 != 0) {
        StageObj_Draw3D(p0, t0);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x128);
    if (t1 != 0) {
        StageObj_Draw3D(p0, t1);
    }
    s32 t2 = *(u32 *)((u8 *)p0 + 0x12c);
    if (t2 != 0) {
        StageObj_Draw3D(p0, t2);
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0xa4);
    s32 t4 = *(u16 *)((u8 *)p0 + 0x5c);
    if (t3 != 0) {
        if (t3 != 0) {
            StageObj_Draw2D(p0, ((u8 *)p0 + 0x94), ((u8 *)p0 + 0xdc));
        }
    }
    *(u16 *)((u8 *)p0 + 0x5c) = t4;
}
