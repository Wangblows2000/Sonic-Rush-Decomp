#include <nitro/types.h>

typedef struct {
    vu32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233877c;

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkPad_0233877c;

extern UnkPlayer_0233877c data_ov000_0236aad8[];
extern UnkPad_0233877c data_022503e0;

void func_ov000_0233877c(void) {
    UnkPlayer_0233877c *p = data_ov000_0236aad8;
    u32 f = p[2].flags;
    u32 g;

    if (f & 1)
        return;
    g = p[2].flags;
    if (g & 0x800)
        return;
    if (!(data_022503e0.unk4 & 4))
        return;
    if (f & 2) {
        p[2].flags = g | 0x200;
        p[2].flags &= ~0x400;
        p[2].flags &= ~2;
    } else {
        p[2].flags = g | 0x200;
        p[2].flags |= 0x400;
        p[2].flags |= 2;
    }
}
