#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    void *unk4;
} UnkStruct_02369b88;

extern u16 data_ov000_02363ddc[];
extern u16 data_ov000_02363dd0[];
extern u8 data_ov000_02363dac[];
extern UnkStruct_02369b88 data_ov000_02369b88;
extern u8 data_ov000_02365c58[];

void Boss_LoadModelPalettes(void *a, void *b, u16 *c, void *d, s32 e);

void BossF_LoadPalette(s32 a, s32 b) {
    u16 *t;

    if (b)
        t = &data_ov000_02363ddc[a];
    else
        t = &data_ov000_02363dd0[a];
    Boss_LoadModelPalettes(data_ov000_02369b88.unk4, data_ov000_02365c58, t, data_ov000_02363dac, 1);
}
