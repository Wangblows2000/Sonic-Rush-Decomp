#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4[2];
    s32 unk8[2];
} UnkStruct_0233e48c;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233e48c;

extern UnkPlayer_0233e48c gCamera[];
extern UnkStruct_0233e48c *gZoneFxWork;
void ZoneFx_Zone4_ResetBldyTimer(UnkStruct_0233e48c *w, u8 i);

void ZoneFx_Zone4_StartBldyFlash(u32 mask) {
    UnkStruct_0233e48c *w;
    u8 i;

    if (!(gCamera[2].flags & 0x100000))
        return;
    w = gZoneFxWork;
    if (w == NULL)
        return;
    for (i = 0; i < 2; i++) {
        if (mask & (1 << i)) {
            ZoneFx_Zone4_ResetBldyTimer(w, i);
            w->unk4[i] = 0x3c;
            w->unk8[i] = 3;
        }
    }
}
