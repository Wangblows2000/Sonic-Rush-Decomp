#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 target[2];
    s32 vel[2];
    s32 acc[2];
    s32 pos[2];
    u8 pad2c[8];
    u32 flags;
    u16 timer;
} UnkObj_02061ac4;

void func_02061a08(UnkObj_02061ac4 *o);
BOOL func_02061a78(UnkObj_02061ac4 *o);

#pragma thumb on
BOOL func_02061ac4(UnkObj_02061ac4 *o) {
    u8 done = 0;
    BOOL ret = FALSE;
    u8 i;

    for (i = 0; i < 2; i++) {
        if (o->flags & (1 << i)) {
            done |= 1 << i;
            continue;
        }
        o->vel[i] += o->acc[i];
        o->pos[i] += o->vel[i];
        if ((o->vel[i] > 0 && o->pos[i] >= o->target[i]) || (o->vel[i] < 0 && o->pos[i] <= o->target[i]) || o->vel[i] == 0) {
            o->pos[i] = o->target[i];
            done |= 1 << i;
        }
    }
    func_02061a08(o);
    if (o->flags & 0x10) {
        o->timer--;
        if (o->timer == 0)
            ret = TRUE;
    } else if (done == 3) {
        ret = TRUE;
    }
    if (ret) {
        o->vel[0] = 0;
        o->vel[1] = 0;
        o->acc[0] = 0;
        o->acc[1] = 0;
        if (o->flags & 4)
            func_02061a78(o);
    }
    return ret;
}
#pragma thumb reset
