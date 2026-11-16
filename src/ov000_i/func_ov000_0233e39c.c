#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x64 - 0xc];
} UnkPlayer_0233e39c;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_0233e39c;

extern s32 *data_ov000_0236ac18;
extern UnkCfg_0233e39c data_022c4560;
extern UnkPlayer_0233e39c data_ov000_0236aad8[];
extern s16 data_0206e470[];

#define SIN_0233e39c(a) data_0206e470[((s32)((u16)(a) & 0xffff) >> 4) * 2]

void func_ov000_0233e39c(s32 x, s32 idx) {
    s32 *w = data_ov000_0236ac18;
    UnkPlayer_0233e39c *p;

    if (w == NULL)
        return;
    if (data_022c4560.unk1C != 5)
        return;
    if (data_022c4560.unk1D == 2)
        return;
    p = &data_ov000_0236aad8[idx];
    if (!(p->flags & 0x80))
        return;
    p->unk8 = x + ((SIN_0233e39c((*w << 7) * 3) << 10) >> 12);
}
