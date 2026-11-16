#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_02341358;

extern u16 data_ov000_0236aab0;
extern UnkPlayer_02341358 gCamera[];

void ZoneBg_SetupBgControl(void) {
    if (data_ov000_0236aab0 & 1) {
        gCamera[0].flags |= 4;
        gCamera[1].flags |= 4;
    } else {
        gCamera[0].flags &= ~4;
        gCamera[1].flags &= ~4;
        return;
    }
    data_ov000_0236aab0 |= 2;
    if (data_ov000_0236aab0 != 0) {
        *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0xe808;
        *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0xe808;
    }
}
