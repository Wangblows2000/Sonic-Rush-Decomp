#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x60 - 4];
    s8 unk60;
    u8 pad61[0x64 - 0x61];
} UnkPlayer_02342744;

extern UnkPlayer_02342744 gCamera[];

void func_ov000_02342744(void) {
    if (gCamera[2].flags & 1) {
        gCamera[0].unk60--;
        return;
    }
    gCamera[0].unk60--;
    gCamera[1].unk60--;
}
