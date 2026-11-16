#include <nitro/types.h>

u8 ObjAngle_MoveToward8(u8 v, s32 a, s32 b);
s16 ObjSpd_DownSet(s32 v, s32 a);

void func_ov000_023475fc(u8 *w) {
    s32 t;

    w[1] = ObjAngle_MoveToward8(w[1], 0, 2);
    if (*(u32 *)(w + 0x330) & 0x1100)
        return;
    t = ObjSpd_DownSet((s16)(*(s32 *)(w + 0x18) >> 8), 4) << 16;
    *(s16 *)(w + 0x32) = ObjSpd_DownSet(*(s16 *)(w + 0x32), 0x20);
    *(s32 *)(w + 0x18) = t >> 8;
    w[2] = ObjAngle_MoveToward8(w[2], 0, 4);
}
