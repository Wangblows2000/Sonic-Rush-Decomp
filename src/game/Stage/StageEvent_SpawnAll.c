#include <nitro/types.h>

typedef struct {
    s32 l;
    s32 t;
    s32 r;
    s32 b;
} UnkStruct_023352c8_R;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    u16 unk10;
} UnkStruct_023352c8_M;

typedef void (*UnkFunc_023352c8)(void *a, u16 x, u16 y, UnkStruct_023352c8_R *r, void *o, u32 f);

extern UnkStruct_023352c8_M data_ov000_0236abd8;
extern UnkFunc_023352c8 data_ov000_0236aa84;
extern UnkFunc_023352c8 gStageEventSpawnerTable[];

void StageEvent_SpawnAll(void *a) {
    UnkStruct_023352c8_R rc;
    u8 i;
    s32 maxx = data_ov000_0236abd8.unkE - 1;
    s32 maxy = data_ov000_0236abd8.unk10 - 1;
    u16 xx;
    u16 yy;

    for (i = 0; i < 3; i++) {
        data_ov000_0236aa84 = gStageEventSpawnerTable[i];
        rc.l = 0;
        rc.t = 0;
        rc.r = maxx;
        rc.b = maxy;
        for (yy = rc.t >> 8; yy <= rc.b >> 8; yy++) {
            for (xx = rc.l >> 8; xx <= rc.r >> 8; xx++) {
                data_ov000_0236aa84(a, xx, yy, &rc, NULL, 0);
            }
        }
    }
}
