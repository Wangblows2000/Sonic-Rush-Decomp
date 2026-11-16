#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 start;
    u8 count;
    s8 dir;
} UnkDesc_0233c9a4;

void func_ov000_0233c9a4(u16 *pal, UnkDesc_0233c9a4 *d) {
    u16 *p = pal + d->start;
    u16 t;
    u8 i;

    if (d->dir > 0) {
        t = p[0];
        for (i = 0; i < d->count - 1; i++)
            p[i] = p[i + 1];
        p[i] = t;
    } else {
        t = p[d->count - 1];
        for (i = d->count - 1; i != 0; i--)
            p[i] = p[i - 1];
        p[i] = t;
    }
}
