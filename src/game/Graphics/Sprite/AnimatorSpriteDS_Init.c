#include <nitro/types.h>
#include "game/Graphics/dual.h"

void MIi_CpuClear16(u16 data, void *dst, u32 size);

void AnimatorSpriteDS_Init(UnkDual *d, UnkSeqRes *res, u16 idx, u32 mask, u32 flags, u32 a0, u32 a1, u32 b0, u32 b1, u32 c0, u32 c1, u32 e0, u32 e1) {
    vu16 zero = 0;
    MIi_CpuClear16(zero, d, sizeof(UnkDual));
    d->sub.res = res;
    d->sub.unk2a = 0x10;
    d->sub.flags = flags;
    d->sub.unk8 = idx;
    d->mask = mask;
    d->unk54[0] = a0;
    d->unk5c[0] = a1;
    d->unk64[0] = b0;
    d->unk6c[0] = b1;
    d->unk54[1] = c0;
    d->unk5c[1] = c1;
    d->unk64[1] = e0;
    d->unk6c[1] = e1;
    d->sub.pos = *(u32 *)((u8 *)res + res->tableOfs + idx * 8 + 4);
}
