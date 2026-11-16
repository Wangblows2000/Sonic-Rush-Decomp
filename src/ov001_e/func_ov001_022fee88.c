#include <nitro/types.h>

typedef struct UnkStruct_022fee88 {
    u8 pad0[0x22];
    u16 masks[12];
} UnkStruct_022fee88;

extern UnkStruct_022fee88 *gExtraBossWork;

u16 func_ov001_022fee88(u32 bits) {
    s32 i;
    u16 r = 0;
    UnkStruct_022fee88 *g = gExtraBossWork;

    for (i = 0; i < 12; i++) {
        if (bits & (1 << i))
            r |= g->masks[i];
    }
    return r;
}
