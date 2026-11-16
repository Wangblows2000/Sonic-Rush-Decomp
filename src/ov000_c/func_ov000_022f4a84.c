#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x45C - 0x20C];
    u16 unk45C;
} UnkStruct_022f4a84;

void func_ov000_022f4a84(UnkStruct_022f4a84 *w, s32 target, s32 range, s32 max, s16 accel, s32 decel, s16 lim) {
    s32 d;
    s32 v = w->base.unk2E;
    s32 x = w->base.unk10;

    d = target - x;
    if (max > range)
        range = max;
    if (v == 0) {
        if (w->unk45C == 0) {
            if (d < 0)
                d = -d;
            if (range < d) {
                if (x < target)
                    w->base.unk2E = max;
                else
                    w->base.unk2E = -max;
            }
        }
    } else {
        if (!((x < target && v < 0) || (x > target && v > 0))) {
            if (d < 0)
                d = -d;
            if (range < d)
                goto accelerate;
        }
        if (v < 0) {
            if (decel < -v)
                w->base.unk2E += decel;
            else
                w->base.unk2E = 0;
        } else if (v > 0) {
            if (decel < v)
                w->base.unk2E -= decel;
            else
                w->base.unk2E = 0;
        }
        if (w->base.unk2E == 0)
            w->unk45C = (Rand() & 0x1f) + 0x1e;
        goto end;
    accelerate:
        if (v > 0)
            w->base.unk2E += accel;
        else
            w->base.unk2E -= accel;
        w->base.unk2E = w->base.unk2E < -lim ? -lim : (w->base.unk2E > lim ? lim : w->base.unk2E);
    }
end:
    if (w->base.unk10 < 0x8000) {
        w->base.unk10 = 0x8000;
        w->base.unk2E = 0;
    }
    if (w->base.unk10 > 0x2c000) {
        w->base.unk10 = 0x2c000;
        w->base.unk2E = 0;
    }
    if (w->unk45C != 0)
        w->unk45C--;
}
