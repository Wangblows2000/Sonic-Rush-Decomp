#include <nitro/types.h>

typedef struct UnkPlayer_0205b478 {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[0x1c - 0x14];
    s32 prevX;
    u8 pad20[0x34 - 0x20];
    s16 unk34;
    u8 pad36[0x358 - 0x36];
    s16 unk358;
    u8 pad35a[0x3ac - 0x35a];
    u16 unk3ac;
    u8 pad3ae[0x420 - 0x3ae];
    u16 unk420;
} UnkPlayer_0205b478;

typedef struct UnkCamera_0205b478 {
    u8 pad0[4];
    s32 x;
    u8 pad8[0x3a - 0x8];
    s8 target;
    u8 pad3b;
    s32 offsetX;
    u8 pad40[0x64 - 0x40];
} UnkCamera_0205b478;

typedef struct UnkCameraMgr_0205b478 {
    UnkCamera_0205b478 cam[2];
    u32 flags;
} UnkCameraMgr_0205b478;

typedef struct UnkTitleCard_0205b478 {
    u8 pad0[4];
    UnkPlayer_0205b478 *player;
    u8 pad8[0x480 - 0x8];
    s32 baseX;
    u8 pad484[4];
    u32 flags;
} UnkTitleCard_0205b478;

extern UnkCameraMgr_0205b478 gCamera;
extern UnkCamera_0205b478 data_ov000_0236ab3c;
extern UnkPlayer_0205b478 *gPlayers[];

static inline void SnapCamera(UnkCamera_0205b478 *cam)
{
    cam->x = gPlayers[cam->target]->x - cam->offsetX;
}

void StageTitleCard_HoldPlayerX(UnkTitleCard_0205b478 *work)
{
    UnkPlayer_0205b478 *player = work->player;
    u32 dx;

    player->unk420 |= 0x10;
    player->unk3ac = 0;
    if (player->unk358 > player->unk34)
        player->unk34 = player->unk358;
    {
        u32 f = work->flags;
        u32 x = player->x;
        dx = x - work->baseX;
        player->prevX = x;
        if (!(f & 0x20000000))
            dx &= 0xffff;
    }
    player->x = work->baseX + dx;
    if (gCamera.flags & 1) {
        gCamera.cam[0].x = gCamera.cam[1].x = player->x - gCamera.cam[0].offsetX;
    } else {
        SnapCamera(&gCamera.cam[0]);
        SnapCamera((UnkCamera_0205b478 *)(u32)&data_ov000_0236ab3c);
    }
}
