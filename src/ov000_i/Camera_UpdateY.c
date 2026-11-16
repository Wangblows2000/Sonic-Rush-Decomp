#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x3a - 0xc];
    s8 unk3A;
    u8 pad3B[0x48 - 0x3b];
    s32 unk48;
    u8 pad4C[0x64 - 0x4c];
} UnkPlayer_023394b4;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkObj_023394b4;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkIn_023394b4;

extern UnkPlayer_023394b4 gCamera[];
extern UnkObj_023394b4 *gPlayers[];

void Camera_ClampYToBounds(UnkIn_023394b4 *in, s32 idx);
BOOL Camera_ClampLookPanY(UnkIn_023394b4 *in, s32 idx);
BOOL Camera_HasReturnedToPlayerY(UnkIn_023394b4 *in, s32 idx);
void Camera_FollowPlayerY(UnkIn_023394b4 *in, s32 idx);
void Camera_Look_Reset(UnkIn_023394b4 *in);

void Camera_UpdateY(UnkIn_023394b4 *in, s32 idx) {
    UnkPlayer_023394b4 *p = &gCamera[idx];

    if (p->flags & 0x4020) {
        Camera_ClampYToBounds(in, idx);
        return;
    }
    if (gCamera[2].flags & 0x10000) {
        if (!(gCamera[2].flags & 0x20000)) {
            p->unk8 += in->unk8;
            Camera_ClampLookPanY(in, idx);
        } else {
            p->unk8 += in->unk8;
            if (in->unk8 != 0 && Camera_HasReturnedToPlayerY(in, idx)) {
                p->unk8 = gPlayers[p->unk3A]->unk14 - p->unk48;
                gCamera[2].flags &= ~0x10000;
                gCamera[2].flags &= ~0x20000;
                Camera_Look_Reset(in);
            }
        }
    } else {
        Camera_FollowPlayerY(in, idx);
    }
    Camera_ClampYToBounds(in, idx);
}
