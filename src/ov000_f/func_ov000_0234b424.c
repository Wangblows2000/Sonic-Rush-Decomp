#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
    u8 pad36[0x5C - 0x36];
    u16 unk5C;
    u8 pad5E[0x328 - 0x5E];
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u8 pad330[0x378 - 0x330];
    s16 unk378;
    s16 unk37A;
    s16 unk37C;
    s16 unk37E;
    s16 unk380;
} UnkStruct_0234b424;

void Player_SetAnimation(UnkStruct_0234b424 *w, s32 state);

void func_ov000_0234b424(UnkStruct_0234b424 *w) {
    s32 a = w->unk34;
    u16 sp;

    if (a < 0) {
        a = -a;
    }
    sp = a;
    if (w->unk328 < 0x3d || w->unk328 > 0x42) {
        Player_SetAnimation(w, 0x3d);
    }
    if (w->unk328 == 0x3d && (w->unk5C & 8) && sp >= w->unk378) {
        Player_SetAnimation(w, 0x3e);
    }
    if (w->unk328 == 0x3e && (w->unk5C & 8)) {
        if (sp >= w->unk37A) {
            Player_SetAnimation(w, 0x3f);
        } else {
            Player_SetAnimation(w, 0x3d);
        }
    }
    if (w->unk328 == 0x3f && (w->unk5C & 8)) {
        if (sp >= w->unk37C) {
            Player_SetAnimation(w, 0x40);
        } else {
            Player_SetAnimation(w, 0x3e);
        }
    }
    if (w->unk32C & 0x180) {
        if (w->unk328 == 0x40 && (w->unk5C & 8)) {
            if (sp >= w->unk37E) {
                Player_SetAnimation(w, 0x41);
            } else {
                Player_SetAnimation(w, 0x3f);
            }
        }
        if (w->unk328 == 0x41 && (w->unk5C & 8)) {
            if (sp >= w->unk380) {
                Player_SetAnimation(w, 0x42);
            } else {
                Player_SetAnimation(w, 0x40);
            }
        }
        if (w->unk328 == 0x42 && (w->unk5C & 8) && sp < w->unk380) {
            Player_SetAnimation(w, 0x41);
        }
    } else if (w->unk328 == 0x40 && (w->unk5C & 8) && sp < w->unk37E) {
        Player_SetAnimation(w, 0x3f);
    }
    w->unk5C |= 4;
}
