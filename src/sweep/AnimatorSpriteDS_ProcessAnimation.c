#include <nitro/types.h>
#include "main_a2/dual.h"

typedef BOOL (*UnkCmd_020332c4)(void *cmd, UnkDualSub *c, u32 a, u32 b);
extern UnkCmd_020332c4 gBACFrameGroupFuncTable[];
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

void AnimatorSpriteDS_ProcessAnimation(UnkDual *d, u32 a, u32 b)
{
    UnkDualSub tmp;
    UnkDualSub *c;
    u32 i;
    UnkDualSub *t;

    if (d->sub.flags & 1) {
        return;
    }
    d->sub.flags &= ~2;
    if (d->sub.wait > 0) {
        d->sub.wait -= d->sub.unk2a;
        d->sub.frame += d->sub.unk2a;
        if (d->sub.wait > 0) {
            return;
        }
        d->sub.frame += d->sub.wait;
        if (d->sub.unkA == d->sub.unkC) {
            d->sub.flags |= 2;
            if (!(d->sub.flags & 4)) {
                d->sub.flags |= 1;
                return;
            }
        }
    }
    d->sub.prevPos = d->sub.pos;
    t = &tmp;
    MIi_CpuCopy16(d, t, sizeof(UnkDualSub));
    c = &d->sub;
    for (i = 0; i < 2; i++) {
        if (!((1 << i) & d->mask)) {
            c->idx = i;
            c->unk30 = d->unk54[i];
            c->unk34 = d->unk5c[i];
            c->unk3c = d->unk64[i];
            c->unk40 = d->unk6c[i];
            c->unk44 = d->unk74[i];
            while (gBACFrameGroupFuncTable[*(u16 *)(c->pos + UnkSeq_GetData(c->res))]((void *)(c->pos + UnkSeq_GetData(c->res)), c, a, b)) {
            }
            a = 0;
            b = 0;
            c = t;
        }
    }
}
