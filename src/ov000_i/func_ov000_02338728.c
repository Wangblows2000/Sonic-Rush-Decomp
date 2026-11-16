#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_02338728;

extern UnkPlayer_02338728 data_ov000_0236aad8[];

void func_ov000_02338728(void) {
    u32 f = data_ov000_0236aad8[2].flags;

    if (!(f & 0x200))
        return;
    if (!(f & 0x400))
        *(vu16 *)0x4000304 |= 0x8000;
    else
        *(vu16 *)0x4000304 &= ~0x8000;
    data_ov000_0236aad8[2].flags &= ~0x200;
}
