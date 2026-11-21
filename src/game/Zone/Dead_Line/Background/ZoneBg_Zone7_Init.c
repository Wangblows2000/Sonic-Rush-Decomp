#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    s32 x;
    s32 y;
    u8 pad34[0x64 - 0x34];
} UnkPlayer_02341044;

typedef struct {
    UnkPlayer_02341044 p0;
    u8 pad64[0xC8 - 0x64];
    u32 flags;
} UnkPlayers_02341044;

extern UnkPlayers_02341044 gCamera;
extern UnkPlayer_02341044 data_ov000_0236ab3c;

UnkPlayer_02341044 *Camera_GetBySwapState(void);
UnkPlayer_02341044 *Camera_GetOtherBySwapState(void);
void ZoneBg_Zone7_TaskMain(void);
void ZoneBg_Zone7_TaskDestructor(void);
void Task_Create(void *a, void *b, s32 c, s32 d, s32 e, s32 f);
#define PLAYER2 ((UnkPlayer_02341044 *)(u32)&data_ov000_0236ab3c)

void ZoneBg_Zone7_Init(void) {
    UnkPlayer_02341044 *p;

    if (gCamera.flags & 1) {
        p = Camera_GetBySwapState();
        p->x = 0;
        p->y = 0;
        p = Camera_GetOtherBySwapState();
        p->x = 0;
        p->y = 0x10000;
    } else {
        gCamera.p0.x = 0;
        gCamera.p0.y = 0;
        PLAYER2->x = 0;
        PLAYER2->y = 0;
    }
    Task_Create(ZoneBg_Zone7_TaskMain, ZoneBg_Zone7_TaskDestructor, 0, 0x3f10, 3, 0);
}
