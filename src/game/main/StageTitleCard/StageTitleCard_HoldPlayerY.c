#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[4];
    s32 prevX;
    s32 prevY;
    u8 pad24[0x34 - 0x24];
    s16 unk34;
    u8 pad36[0x358 - 0x36];
    s16 unk358;
    u8 pad35A[0x3ac - 0x35a];
    u16 unk3AC;
    u8 pad3AE[0x420 - 0x3ae];
    u16 unk420;
} UnkPlayer_0205b3b8;

typedef struct {
    u8 pad0[4];
    s32 x;
    s32 y;
    u8 padC[0x3a - 0xc];
    s8 target;
    u8 pad3B[1];
    s32 ofsX;
    s32 ofsY;
    u8 pad44[0xc8 - 0x44];
    u32 flags;
} UnkCam_0205b3b8;

typedef struct {
    u8 pad0[4];
    UnkPlayer_0205b3b8 *player;
    u8 pad8[0x480 - 8];
    s32 baseX;
    s32 baseY;
    u32 flags;
} UnkWork_0205b3b8;

extern UnkCam_0205b3b8 gCamera;
extern UnkCam_0205b3b8 data_ov000_0236ab3c;
extern UnkPlayer_0205b3b8 *gPlayers[];

static inline void CamFollow_0205b3b8(UnkCam_0205b3b8 *cam) {
    cam->y = gPlayers[cam->target]->y - cam->ofsY;
}

void StageTitleCard_HoldPlayerY(UnkWork_0205b3b8 *w) {
    UnkPlayer_0205b3b8 *p = w->player;
    s32 d;
    u32 flags;
    UnkCam_0205b3b8 *cam;

    p->unk420 = 0;
    p->unk3AC = 0;
    if (p->unk358 > p->unk34)
        p->unk34 = p->unk358;
    flags = w->flags;
    d = p->y - w->baseY;
    p->prevY = p->y;
    if (!(flags & 0x20000000)) d &= 0xffff;
    p->y = w->baseY + d;
    if (gCamera.flags & 1) {
        gCamera.y = p->y - gCamera.ofsY;
        return;
    }
    CamFollow_0205b3b8((UnkCam_0205b3b8 *)(u32)&gCamera);
    CamFollow_0205b3b8((UnkCam_0205b3b8 *)(u32)&data_ov000_0236ab3c);
}
