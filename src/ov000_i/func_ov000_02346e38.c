#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x10 - 1];
    s32 x;
    s32 y;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
} UnkObj_02346e38;

typedef struct {
    u8 pad0[0xa];
    u16 unkA;
    u8 padC[0x18 - 0xc];
    UnkObj_02346e38 *obj;
} UnkTask_02346e38;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x32c - 0x18];
    u32 unk32C;
    u8 pad330[0x3dc - 0x330];
    UnkObj_02346e38 *target;
} UnkStruct_02346e38;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02346e38;

extern UnkCfg_02346e38 data_022c4560;
UnkTask_02346e38 *ObjRect_RegistGet(s32 a, u8 b);

void func_ov000_02346e38(UnkStruct_02346e38 *w) {
    UnkTask_02346e38 *t;
    UnkObj_02346e38 *o;
    s32 dx, dy, d;
    u8 i;
    UnkObj_02346e38 *best = NULL;
    s32 bestd = 0x7fffffff;

    if (w->target != NULL && (w->target->unk54 & 4))
        w->target = NULL;
    if (w->unk32C & 0x40)
        return;
    if (data_022c4560.unk1C >= 9)
        return;
    if (data_022c4560.unk1D >= 2)
        return;
    i = 0;
    while (1) {
        t = ObjRect_RegistGet(2, i);
        if (t == NULL)
            break;
        if (!(t->unkA & 0x1000) && (o = t->obj) != NULL && o != NULL && o->unk0 == 2) {
            dy = (o->y - w->y) >> 8;
            dx = (o->x - w->x) >> 8;
            d = dx * dx + dy * dy;
            if (bestd > d) {
                bestd = d;
                best = o;
            }
        }
        i++;
    }
    w->target = NULL;
    w->target = best;
}
