#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    s32 x;
    s32 y;
    u8 pad34[0x64 - 0x34];
} UnkPlayer_023410dc;

typedef struct {
    UnkPlayer_023410dc p0;
    u8 pad64[0xC8 - 0x64];
    u32 flags;
} UnkPlayers_023410dc;

extern UnkPlayers_023410dc gCamera;
extern UnkPlayer_023410dc data_ov000_0236ab3c;

UnkPlayer_023410dc *Camera_GetBySwapState(void);
UnkPlayer_023410dc *Camera_GetOtherBySwapState(void);

#define PLAYER2 ((UnkPlayer_023410dc *)(u32)&data_ov000_0236ab3c)

void ZoneBg_Zone6_Init(void) {
    UnkPlayer_023410dc *p;

    if (gCamera.flags & 1) {
        p = Camera_GetBySwapState();
        p->x = 0;
        p->y = 0x3000;
        p = Camera_GetOtherBySwapState();
        p->x = 0;
        p->y = 0x14000;
    } else {
        gCamera.p0.x = 0;
        gCamera.p0.y = 0x3000;
        PLAYER2->x = 0;
        PLAYER2->y = 0x3000;
    }
}
