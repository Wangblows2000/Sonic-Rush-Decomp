/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
extern u8 data_ov000_0236abec[];

s32 func_ov000_02339364(s32 p0, s32 p1) {
    s32 t0 = *(u32 *)data_ov000_0236abec;
    s32 t1 = *(u32 *)((u8 *)((s32)p1 * 0x64 + (s32)gCamera) + 0x4);
    s32 v2 = t0;
    if (t1 >= t0) {
        s32 t3 = (*(u32 *)((u8 *)data_ov000_0236abec + 0x8) - (*(s16 *)((u8 *)((s32)p1 * 0x64 + (s32)gCamera) + 0x1e) << 8));
        s32 v4 = t3;
        if (t1 <= t3) {
            v4 = t1;
        }
        v2 = v4;
    }
    *(u32 *)((u8 *)((s32)p1 * 0x64 + (s32)gCamera) + 0x4) = v2;
    return v2;
}
