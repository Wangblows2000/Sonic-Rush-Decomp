#include <nitro/types.h>

typedef struct UnkStruct_0230ed48 {
    u8 pad0[0x20];
    s32 x;
    s32 y;
    s32 z;
    u8 pad2C[0xd0 - 0x2c];
    s32 timer;
} UnkStruct_0230ed48;

extern u32 gRandSeed;

void *SpecialStage_GetPlayer(void);
void func_ov001_02312dc8(void *a, UnkStruct_0230ed48 *p);
void SpecialStageActObj_SpawnAt(s32 type, s32 x, s32 y, s32 z);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_0230ed48(UnkStruct_0230ed48 *p) {
    func_ov001_02312dc8(SpecialStage_GetPlayer(), p);
    if (p->timer % 17 == 1)
        SpecialStageActObj_SpawnAt(2, p->x + Rand() % 0x4000 - 0x2000, p->y + Rand() % 0x4000 - 0x2000,
                            p->z + Rand() % 0x4000 - 0x2000);
}
