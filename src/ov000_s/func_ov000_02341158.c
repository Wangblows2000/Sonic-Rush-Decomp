#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    s32 x;
    s32 y;
    u8 pad34[0x64 - 0x34];
} UnkPlayer_02341158;

typedef struct {
    UnkPlayer_02341158 p0;
    u8 pad64[0xC8 - 0x64];
    u32 flags;
} UnkPlayers_02341158;

extern UnkPlayers_02341158 gCamera;
extern UnkPlayer_02341158 data_ov000_0236ab3c;

UnkPlayer_02341158 *func_ov000_0233b62c(void);
UnkPlayer_02341158 *func_ov000_0233b610(void);
void func_ov000_0233f67c(void);
void func_ov000_0233f6f4(void);
void Task_Create(void *a, void *b, s32 c, s32 d, s32 e, s32 f);
#define PLAYER2 ((UnkPlayer_02341158 *)(u32)&data_ov000_0236ab3c)

void func_ov000_02341158(void) {
    UnkPlayer_02341158 *p;

    if (gCamera.flags & 1) {
        p = func_ov000_0233b62c();
        p->x = 0;
        p->y = 0;
        p = func_ov000_0233b610();
        p->x = 0;
        p->y = 0x10000;
    } else {
        gCamera.p0.x = 0;
        gCamera.p0.y = 0;
        PLAYER2->x = 0;
        PLAYER2->y = 0;
    }
    Task_Create(func_ov000_0233f67c, func_ov000_0233f6f4, 0, 0x3f10, 3, 0);
}
