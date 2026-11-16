#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x20 - 0xc];
    s16 unk20;
    u8 pad22[0x62 - 0x22];
    u16 unk62;
} UnkPlayer_023427bc;

extern UnkPlayer_023427bc data_ov000_0236aad8[];

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

u8 func_ov000_023427bc(s32 idx) {
    UnkPlayer_023427bc *p = &data_ov000_0236aad8[idx];
    s16 v;

    if (p->flags & 0x1000000) {
        v = ((p->unk62 << 8) - p->unk8) / p->unk20;
        return CLAMP(v, 0, 0xc0);
    }
    return 0xc0;
}
