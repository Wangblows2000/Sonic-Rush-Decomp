#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x1c - 0x14];
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x64 - 0x1e];
} UnkPlayer_02339154;

extern UnkPlayer_02339154 data_ov000_0236aad8[];

void func_ov000_02339154(s32 unused, s32 idx) {
    UnkPlayer_02339154 *p = &data_ov000_0236aad8[idx];

    if (data_ov000_0236aad8[2].flags & 0x200)
        return;
    if (!(p->flags & 0x200000))
        return;
    if (p->unk8 > (p->unk1D << 8) + p->unk10) {
        p->unk8 = (p->unk1D << 8) + p->unk10;
        return;
    }
    if (p->unk8 < p->unk10 - (p->unk1D << 8))
        p->unk8 = p->unk10 - (p->unk1D << 8);
    else
        p->flags |= 0x800000;
}
