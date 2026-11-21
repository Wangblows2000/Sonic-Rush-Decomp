#include <nitro/types.h>

typedef struct {
    u8 flags[7][3];
    u8 pad15;
    u16 times[7][3][5];
    u8 bossFlags[2][2];
    u16 bossTimes[2][2][5];
} UnkSave_022e9c24;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 cleared;
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 d4;
    s32 valid;
} UnkOut_022e9c24;

extern s32 gGameState;
extern u8 data_ov001_023166c4[];
extern u8 data_ov001_023166bc[];
extern UnkSave_022e9c24 data_022c473c;

#pragma thumb on
void TimeAttackMenu_GetRecordTimes(s32 a, s32 b, UnkOut_022e9c24 *out) {
    s32 q, r;
    s32 i;
    s32 t;

    if (a >= 7) {
        q = b / 2;
        r = b % 2;
    } else if (gGameState == 0) {
        a = data_ov001_023166c4[a];
    } else {
        a = data_ov001_023166bc[a];
    }
    for (i = 0; i < 5; i++) {
        if (a >= 7) {
            if (data_022c473c.bossFlags[q][r] & (1 << i)) {
                out->cleared = 1;
            } else {
                out->cleared = 0;
            }
        } else {
            if (data_022c473c.flags[a][b] & (1 << i)) {
                out->cleared = 1;
            } else {
                out->cleared = 0;
            }
        }
        if (a >= 7) {
            t = data_022c473c.bossTimes[q][r][i];
        } else {
            t = data_022c473c.times[a][b][i];
        }
        if (t > 0x8c9f) {
            out->valid = 0;
            out->d4 = 10;
            out->d3 = 10;
            out->d2 = 10;
            out->d1 = 10;
            out->d0 = 10;
        } else {
            out->valid = 1;
            out->d4 = t / 3600;
            t = t % 3600;
            out->d3 = t / 600;
            t = t % 600;
            out->d2 = t / 60;
            t = t % 60;
            t = (t * 0x1b1) >> 8;
            if (t >= 100) t = 99;
            out->d1 = t / 10;
            t = t % 10;
            out->d0 = t;
        }
        out++;
    }
}
#pragma thumb reset
