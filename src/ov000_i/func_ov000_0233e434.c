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

extern void *data_ov000_0236ac18;
extern UnkCfg_0233e434 data_022c4560;
extern UnkPlayer_0233e434 data_ov000_0236aad8[];

void func_ov000_0233e434(s32 x, s32 idx) {
    UnkPlayer_0233e434 *p;

    if (data_ov000_0236ac18 == NULL)
        return;
    if (data_022c4560.unk1C != 5)
        return;
    if (data_022c4560.unk1D == 2)
        return;
    p = &data_ov000_0236aad8[idx];
    if (p->flags & 0x40)
        p->unk4 = x;
}
