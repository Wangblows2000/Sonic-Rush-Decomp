#include <nitro/types.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 flags;
} UnkPlayer_02065ea8;

u8 func_02065ea8(UnkPlayer_02065ea8 *p) {
    u8 layer = 0;
    if (!(p->flags & 0x100000))
        layer |= 0x80;
    if (p->unk54 & 1)
        layer |= 1;
    if (p->flags & 0x80000)
        layer |= 0x40;
    return layer;
}
