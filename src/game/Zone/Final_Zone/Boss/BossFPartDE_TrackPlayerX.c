#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
} UnkStruct_022f6b8c;

extern UnkStruct_022f6b8c *gPlayers;

void BossFPartDE_TrackPlayerX(UnkStruct_022f6b8c *s) {
    s32 t;
    s32 d;
    s16 a;

    t = gPlayers->unk10;
    if (t < 0x8000) {
        t = 0x8000;
    } else if (t > 0x2c000) {
        t = 0x2c000;
    }
    d = t - s->unk10;
    if ((d < 0 ? -d : d) < 0x3000) {
        a = s->unk2E;
        if (a < 0) {
            if (-a > 0x60) {
                s->unk2E = a + 0x60;
            } else {
                s->unk2E = 0;
            }
        } else if (a > 0) {
            if (a > 0x60) {
                s->unk2E = a - 0x60;
            } else {
                s->unk2E = 0;
            }
        }
    } else if (d > 0) {
        a = s->unk2E;
        if (a < 0x800) {
            if (0x800 - a > 0x60) {
                s->unk2E = a + 0x60;
            } else {
                s->unk2E = 0x800;
            }
        } else if (a > 0x800) {
            if (a - 0x800 > 0x60) {
                s->unk2E = a - 0x60;
            } else {
                s->unk2E = 0x800;
            }
        }
    } else {
        a = s->unk2E;
        if (a < -0x800) {
            if (-0x800 - a > 0x60) {
                s->unk2E = a + 0x60;
            } else {
                s->unk2E = -0x800;
            }
        } else if (a > -0x800) {
            if (a + 0x800 > 0x60) {
                s->unk2E = a - 0x60;
            } else {
                s->unk2E = -0x800;
            }
        }
    }
}
