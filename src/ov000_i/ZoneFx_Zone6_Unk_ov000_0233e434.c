#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 unk4;
    u8 pad8[0x64 - 8];
} UnkPlayer_0233e434;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_0233e434;

extern void *gZoneFxWork;
extern UnkCfg_0233e434 gGameState;
extern UnkPlayer_0233e434 gCamera[];

void ZoneFx_Zone6_Unk_ov000_0233e434(s32 x, s32 idx) {
    UnkPlayer_0233e434 *p;

    if (gZoneFxWork == NULL)
        return;
    if (gGameState.unk1C != 5)
        return;
    if (gGameState.unk1D == 2)
        return;
    p = &gCamera[idx];
    if (p->flags & 0x40)
        p->unk4 = x;
}
