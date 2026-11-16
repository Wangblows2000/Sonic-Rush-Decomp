#include <nitro/types.h>

typedef struct { u8 pad0[4]; u8 *mdl; } UnkObj_0205615c;
typedef struct {
    u8 pad0[4];
    UnkObj_0205615c *obj;
    u32 flags;
    u8 padC[0xa2];
    u8 unkAE;
} UnkWork_0205615c;
extern u16 data_022c4674;
extern u8 data_020756e4[];
extern u8 data_020756f4[];

s32 func_02025edc(void *a, void *b);
void func_02025268(s32 id, s32 *val, s32 flag);

#pragma thumb on
void func_0205615c(UnkWork_0205615c *w) {
    u8 i;
    s32 r;
    s32 v;
    s32 cmp;

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            r = func_02025edc(w->obj->mdl + 0x40, data_020756e4);
            v = 0x1e;
        } else {
            r = func_02025edc(w->obj->mdl + 0x40, data_020756f4);
            v = 0x1d;
        }
        if (r >= 0) {
            if (w->flags & 0x10)
                cmp = w->unkAE;
            else
                cmp = -1;
            if (r == cmp) {
                s32 tmp;
                data_022c4674 |= 1 << i;
                tmp = v;
                func_02025268(0x13, &tmp, 1);
            }
        }
    }
}
#pragma thumb reset
