#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad210[0xD5E - 0x210];
    u16 unkD5E;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0xF68];
    s16 unkF68;
    u8 padF6A[0x1128 - 0xF6A];
    s32 unk1128;
} UnkStruct_022e37f0;

extern UnkStruct_Boss *gBoss6;

void Boss6_CreatePiece(s32 a, s32 x, s32 y, s32 z);

void Boss6_SpawnDebrisTimed(UnkStruct_022e37f0 *s) {
    UnkStruct_Boss *b;
    s32 r;
    s32 x;
    s32 a;
    s32 c;
    s32 z;

    s->unk1128--;
    if (s->unk1128 > 0)
        return;
    s->unk1128 = (0x549f - s->unkF68) * 10 / 0x549f + 5;
    r = Rand() & 0x1f;
    b = gBoss6;
    if (b->unkD5E != 0) {
        if (b->unk20C & 0x100)
            x = 0x328 - r;
        else
            x = r + 0x18;
    } else {
        if (b->unk20C & 0x100)
            x = r + 0x18;
        else
            x = 0x328 - r;
    }
    a = (0x1ec - (Rand() & 0x3f)) << 8;
    z = ((Rand() & 0x3f) + 0x1e) << 8;
    if (Rand() & 1)
        z *= -1;
    Boss6_CreatePiece(1, x << 8, a, z);
}
