/* paramclone of Boss3_LoadPalettes (src/ov000_a/Boss3_LoadPalettes.c); immediates and relocations remapped. */
#include <nitro/types.h>

extern s32 data_ov000_02369b88[];
extern u16 data_ov000_023640ec[][13];
extern u16 data_ov000_023640b8[][13];
extern u32 data_ov000_02365d6c[];
extern u16 data_ov000_02363f60[];

void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void BossF_Unk_ov000_022f8c88(s32 k, BOOL f) {
    u16 *t;
    if (f)
        t = data_ov000_023640ec[k];
    else
        t = data_ov000_023640b8[k];
    Boss_LoadModelPalettes(data_ov000_02369b88[1], data_ov000_02365d6c, t, data_ov000_02363f60, 13);
}
