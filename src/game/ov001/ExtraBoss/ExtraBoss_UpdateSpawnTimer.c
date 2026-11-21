#include <nitro/types.h>

typedef struct UnkStruct_02304834 {
    u8 pad0[0xb3c];
    s32 timer;
    s32 count;
} UnkStruct_02304834;

extern UnkStruct_02304834 *data_ov001_0231f54c;
extern s32 data_ov001_0231f520;
extern u32 gRandSeed;

s32 FX_ModS32(s32 n, s32 d);
void ExtraBoss_SpawnMeteorR(s32 a, s32 x, s32 y, s32 b);
void ExtraBoss_SpawnMeteor(s32 a, s32 x, s32 y, s32 b);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraBoss_UpdateSpawnTimer(void) {
    s32 y;
    s32 s;

    data_ov001_0231f54c->timer--;
    if (data_ov001_0231f54c->timer > 0)
        return;
    y = FX_ModS32(Rand(), 6) << 17;
    s = -data_ov001_0231f520;
    data_ov001_0231f54c->count++;
    y = y + 0x10000 + (s & ~0x1f);
    if (data_ov001_0231f54c->count >= 10) {
        data_ov001_0231f54c->count = 0;
        ExtraBoss_SpawnMeteorR(0, 0x108000, y, 0);
    } else {
        ExtraBoss_SpawnMeteor(0, 0x108000, y, 0);
    }
    data_ov001_0231f54c->timer = (Rand() & 0x1f) + 0x14;
}
