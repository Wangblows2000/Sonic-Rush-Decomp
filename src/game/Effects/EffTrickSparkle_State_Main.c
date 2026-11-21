#include <game/Stage/stageobj_alt.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0232c658_P;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0232c658_V;

typedef struct {
    u8 pad0[0x494];
    UnkStruct_0232c658_P pos[32];
    UnkStruct_0232c658_V vel[32];
    s16 state[32];
} UnkStruct_0232c658;

void EffTrickSparkle_State_Main(UnkStruct_0232c658 *w) {
    s16 i;

    for (i = 0; i < 32; i++) {
        if (w->state[i] != 0) {
            w->vel[i].y += 0x30;
            if (w->vel[i].y > 0)
                w->vel[i].y += 0x18;
            if (!(Rand() & 0x1f))
                w->vel[i].x = -w->vel[i].x;
            if (!(Rand() & 0x1f))
                w->vel[i].y = 0;
            w->pos[i].x += w->vel[i].x;
            w->pos[i].y += w->vel[i].y;
            if ((w->pos[i].y >> 8) > 0xc4 && w->vel[i].y <= 0)
                w->state[i] = 0;
        }
    }
}
