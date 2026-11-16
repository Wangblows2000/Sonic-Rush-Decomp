#include <nitro/types.h>

extern s32 gGameState[];
extern u16 data_ov000_02363514[];
extern u8 data_ov000_02363504;
extern u16 data_ov000_0236350c[];
extern void *data_ov000_02369b30[];
extern u8 data_ov000_023655e8;

void Boss_LoadModelPalettes(void *a, void *b, void *c, void *d, s32 e);

void Boss4_Unk_ov000_022db9ec(BOOL flag) {
    s32 i = gGameState[0];
    u16 *p;
    if (flag)
        p = &data_ov000_02363514[i];
    else
        p = &data_ov000_0236350c[i];
    Boss_LoadModelPalettes(data_ov000_02369b30[0x14 / 4], &data_ov000_023655e8, p, &data_ov000_02363504, 1);
}
