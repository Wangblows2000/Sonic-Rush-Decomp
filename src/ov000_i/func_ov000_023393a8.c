#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x20 - 0xc];
    s16 unk20;
    u8 pad22[0x50 - 0x22];
    s32 unk50;
    u8 pad54[4];
    s32 unk58;
    u8 pad5C[0x64 - 0x5c];
} UnkPlayer_023393a8;

extern UnkPlayer_023393a8 data_ov000_0236aad8[];

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void func_ov000_023393a8(s32 unused, s32 idx) {
    UnkPlayer_023393a8 *p = &data_ov000_0236aad8[idx];
    u32 flags = data_ov000_0236aad8[2].flags;

    if (flags & 0x200)
        return;
    if (flags & 1) {
        if (!(flags & 2)) {
            p->unk8 = CLAMP(p->unk8, p->unk50, p->unk58 - p->unk20 * 0x1d0);
            return;
        }
        p->unk8 = CLAMP(p->unk8, p->unk50 + p->unk20 * 0x110, p->unk58 - p->unk20 * 0xc0);
        return;
    }
    p->unk8 = CLAMP(p->unk8, p->unk50, p->unk58 - p->unk20 * 0xc0);
}
