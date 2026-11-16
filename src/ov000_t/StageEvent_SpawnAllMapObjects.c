#include <nitro/types.h>

typedef struct {
    s32 l;
    s32 t;
    s32 r;
    s32 b;
} UnkStruct_02335704_R;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    u16 unk10;
} UnkStruct_02335704_M;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_02335704_Task;

extern UnkStruct_02335704_M data_ov000_0236abd8;
extern UnkStruct_02335704_Task *gStageEventManagerTask;

void StageEvent_SpawnMapObjectsInBlock(void *a, u16 x, u16 y, UnkStruct_02335704_R *r, void *o, u32 f);
void StageEvent_SpawnAllMapObjects(void) {
    UnkStruct_02335704_R rc;
    s32 maxx; s32 maxy; u16 cols; void *work; u16 rows; u16 xx; u16 yy;
    maxx = data_ov000_0236abd8.unkE - 1;
    work = gStageEventManagerTask->work;
    cols = (maxx >> 8) + 1;
    maxy = data_ov000_0236abd8.unk10 - 1;
    rows = (maxy >> 8) + 1;
    rc.l = 0; rc.r = maxx; rc.t = 0; rc.b = maxy;
    yy = 0;
    for (; yy < rows; yy++) {
        for (xx = 0; xx < cols; xx++)
            StageEvent_SpawnMapObjectsInBlock(work, xx, yy, &rc, NULL, 8);
    }
}
