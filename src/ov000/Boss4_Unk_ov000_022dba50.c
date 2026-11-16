#include <nitro/types.h>

extern s32 gGameState[];
extern u32 data_ov000_02363528[];
extern u8 data_ov000_02363510;
extern u32 data_ov000_02363520[];
extern void *data_ov000_02369b30[];
extern u8 data_ov000_023655f0;

void Boss_LoadModelPalettes(void *a, void *b, void *c, void *d, s32 e);

void Boss4_Unk_ov000_022dba50(BOOL flag) {
    s32 i = gGameState[0];
    u32 *p;
    if (flag)
        p = &data_ov000_02363528[i];
    else
        p = &data_ov000_02363520[i];
    Boss_LoadModelPalettes(data_ov000_02369b30[0x10 / 4], &data_ov000_023655f0, p, &data_ov000_02363510, 2);
}
