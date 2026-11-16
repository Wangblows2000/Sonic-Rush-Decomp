#include <nitro/types.h>
#include "main_a2/dual.h"

typedef BOOL (*UnkCmd_020331f8)(void *cmd, UnkDualSub *c, u32 a, u32 b);
extern UnkCmd_020331f8 data_02072e00[];

void func_020331f8(UnkDualSub *c) {
    UnkDualSub tmp;
    if ((c->flags & 0x18) == 0x18) {
        return;
    }
    if (c->prevPos == 0) {
        return;
    }
    tmp = *c;
    tmp.pos = tmp.prevPos;
    while (data_02072e00[*(u16 *)(tmp.pos + UnkSeq_GetData(tmp.res))]((void *)(tmp.pos + UnkSeq_GetData(tmp.res)), &tmp, 0, 0)) {
    }
}
