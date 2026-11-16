#pragma opt_common_subs off
#pragma opt_propagation off
#pragma opt_strength_reduction off
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_0233c6ac;

extern UnkPlayer_0233c6ac gCamera[];
void ZoneFx_Zone5_ApplyPaletteAdd(u8 *w, s32 i);
void ZoneFx_Zone5_Unk_ov000_0233c5cc(u8 *w, s32 i);

void ZoneFx_Zone5_UpdatePalettes(u8 *w) {
    s32 i;
    u8 *p;

    for (i = 0; i < 2; i++) {
        if (gCamera[i].unk34 & 0x10000) {
            p = w + i;
            p[0x202]++;
            if (p[0x202] >= 2) {
                p[0x202] = 0;
                p[0x200]++;
                if (p[0x200] >= 0x1e)
                    p[0x200] = 0;
            }
            ZoneFx_Zone5_ApplyPaletteAdd(w, i);
        } else if (w[i + 0x200] != 0) {
            ZoneFx_Zone5_Unk_ov000_0233c5cc(w, i);
            w[i + 0x200] = 0;
            w[i + 0x202] = 0;
        }
    }
}
