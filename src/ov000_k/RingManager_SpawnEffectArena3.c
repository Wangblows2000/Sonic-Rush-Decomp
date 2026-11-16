#include <nitro/types.h>

typedef struct {
    u8 pad0[0x144];
    s32 unk144;
} UnkStruct_0235a848;

void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void RingEffect3D_Create(s32 x, s32 y, s32 z, s32 w);

void RingManager_SpawnEffectArena3(UnkStruct_0235a848 *w, s32 *pos) {
    s32 ox, oy;
    BossArena_ArcToCirclePos(0x128fb8, 0x749fed, pos[0] << 4, &ox, &oy, NULL);
    RingEffect3D_Create(ox, -pos[1] << 4, oy, w->unk144);
}
