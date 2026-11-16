#include <nitro/types.h>
#include "main_a2/dual.h"

typedef BOOL (*UnkCmd_0203344c)(void *cmd, UnkDualSub *c, u32 a, u32 b);
extern UnkCmd_0203344c data_02072e00[];

void func_0203344c(UnkDualSub *c, u32 a, u32 b) {
    if (c->flags & 1) {
        return;
    }
    c->flags &= ~2;
    if (c->wait > 0) {
        c->wait -= c->unk2a;
        c->frame += c->unk2a;
        if (c->wait > 0) {
            return;
        }
        c->frame += c->wait;
        if (c->unkA == c->unkC) {
            c->flags |= 2;
            if (!(c->flags & 4)) {
                c->flags |= 1;
                return;
            }
        }
    }
    c->prevPos = c->pos;
    while (data_02072e00[*(u16 *)(c->pos + UnkSeq_GetData(c->res))]((void *)(c->pos + UnkSeq_GetData(c->res)), c, a, b)) {
    }
}
