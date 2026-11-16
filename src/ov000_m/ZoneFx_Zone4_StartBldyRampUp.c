#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4[2];
    s32 unk8[2];
} UnkStruct_0233e5c8;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233e5c8;

extern UnkPlayer_0233e5c8 gCamera[];
extern UnkStruct_0233e5c8 *gZoneFxWork;
void ZoneFx_Zone4_ResetBldyTimer(UnkStruct_0233e5c8 *w, u8 i);

void ZoneFx_Zone4_StartBldyRampUp(u32 mask, s32 n) {
    UnkStruct_0233e5c8 *w;
    u8 i;
    u16 t;

    if (!(gCamera[2].flags & 0x100000))
        return;
    w = gZoneFxWork;
    if (w == NULL)
        return;
    if (n <= 0)
        n = 1;
    t = n;
    for (i = 0; i < 2; i++) {
        if (mask & (1 << i)) {
            ZoneFx_Zone4_ResetBldyTimer(w, i);
            w->unk4[i] = t;
            w->unk8[i] = 1;
        }
    }
}
