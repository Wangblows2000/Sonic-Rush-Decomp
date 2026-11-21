#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    u8 pad20[0x325 - 0x20];
    u8 unk325;
} UnkStruct_02343ac0;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02343ac0_4560;

typedef struct {
    u32 flags;
    s32 x;
    u8 pad8[0x68 - 8];
    s32 unk68;
} UnkStruct_02343ac0_Cam;

extern UnkStruct_02343ac0_4560 gGameState;
extern UnkStruct_02343ac0_Cam gCamera;
extern s32 data_ov000_0236ac2c;

void EffTrail_SetWrapShift(s32 d);
void StageEvent_SpawnInRect(u16 a, u16 b, u16 c, u16 d);

void Player_WrapLoopingArena(UnkStruct_02343ac0 *w) {
    BOOL ok;
    s32 lo;
    s32 hi;
    s32 d;
    s32 bottom;
    s32 right;
    s32 top;
    s32 left;
    s32 b2;
    s32 r2;
    s32 t2;
    s32 l2;

    ok = FALSE;
    if (gGameState.unk1C >= 7 && gGameState.unk1C < 9 && gGameState.unk1D < 2) {
        ok = TRUE;
    }
    if (!ok) {
        return;
    }
    switch (gGameState.unk1D) {
    case 0:
        if (gGameState.unk1C == 8) {
            return;
        }
        lo = 0x10000;
        hi = 0xb0000;
        break;
    case 1:
        if (gGameState.unk1C == 8) {
            lo = 0x10000;
            hi = 0x180000;
        } else {
            lo = 0x10000;
            hi = 0xb0000;
        }
        break;
    }
    if (w->x >= hi) {
        d = hi - lo;
        w->x -= d;
        w->unk1C -= d;
        if (w->unk325 == 0) {
            data_ov000_0236ac2c -= d >> 8;
            gCamera.x -= d;
            gCamera.unk68 -= d;
            EffTrail_SetWrapShift(-d);
            left = (w->x >> 8) - 0x100;
            top = (w->y >> 8) - 0x120;
            right = (w->x >> 8) + 0x100;
            bottom = (w->y >> 8) + 0x1e0;
            if (left < 0) {
                left = 0;
            } else if (left > 0xffff) {
                left = 0xffff;
            }
            if (top < 0) {
                top = 0;
            } else if (top > 0xffff) {
                top = 0xffff;
            }
            if (right < 0) {
                right = 0;
            } else if (right > 0xffff) {
                right = 0xffff;
            }
            if (bottom < 0) {
                bottom = 0;
            } else if (bottom > 0xffff) {
                bottom = 0xffff;
            }
            StageEvent_SpawnInRect(left, top, right, bottom);
        }
    }
    if (w->x >= lo) {
        return;
    }
    d = hi - lo;
    w->x += d;
    w->unk1C += d;
    if (w->unk325 != 0) {
        return;
    }
    data_ov000_0236ac2c += d >> 8;
    gCamera.x += d;
    gCamera.unk68 += d;
    EffTrail_SetWrapShift(d);
    l2 = (w->x >> 8) - 0x100;
    t2 = (w->y >> 8) - 0x120;
    r2 = (w->x >> 8) + 0x100;
    b2 = (w->y >> 8) + 0x1e0;
    if (l2 < 0) {
        l2 = 0;
    } else if (l2 > 0xffff) {
        l2 = 0xffff;
    }
    if (t2 < 0) {
        t2 = 0;
    } else if (t2 > 0xffff) {
        t2 = 0xffff;
    }
    if (r2 < 0) {
        r2 = 0;
    } else if (r2 > 0xffff) {
        r2 = 0xffff;
    }
    if (b2 < 0) {
        b2 = 0;
    } else if (b2 > 0xffff) {
        b2 = 0xffff;
    }
    StageEvent_SpawnInRect(l2, t2, r2, b2);
}
