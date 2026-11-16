#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    s32 x;
    s32 y;
    u8 pad34[0x64 - 0x34];
} UnkPlayer_023411f0;

typedef struct {
    UnkPlayer_023411f0 p0;
    u8 pad64[0xC8 - 0x64];
    u32 flags;
} UnkPlayers_023411f0;

extern UnkPlayers_023411f0 gCamera;
extern UnkPlayer_023411f0 data_ov000_0236ab3c;

UnkPlayer_023411f0 *func_ov000_0233b62c(void);
UnkPlayer_023411f0 *func_ov000_0233b610(void);

#define PLAYER2 ((UnkPlayer_023411f0 *)(u32)&data_ov000_0236ab3c)

void func_ov000_023411f0(void) {
    UnkPlayer_023411f0 *p;

    if (gCamera.flags & 1) {
        p = func_ov000_0233b62c();
        p->x = 0;
        p->y = 0;
        p = func_ov000_0233b610();
        p->x = 0;
        p->y = 0x11000;
    } else {
        gCamera.p0.x = 0;
        gCamera.p0.y = 0;
        PLAYER2->x = 0;
        PLAYER2->y = 0;
    }
}
