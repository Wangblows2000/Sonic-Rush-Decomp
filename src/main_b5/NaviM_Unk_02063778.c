#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 cmdOfs;
} UnkRes_02063778;

typedef struct UnkAnim_02063778 {
    u8 pad0[0xe];
    s16 time;
    UnkRes_02063778 *res;
    u8 pad14[8];
    u32 cur;
    u32 prev;
    u8 pad24[4];
    s16 wait;
    u8 pad2a[2];
    u32 flags;
} UnkAnim_02063778;

typedef BOOL (*UnkCmd_02063778)(void *cmd, UnkAnim_02063778 *a, s32 b, s32 c);

extern UnkCmd_02063778 gBACFrameGroupFuncTable[];

void NaviM_Unk_02063778(UnkAnim_02063778 *a, s16 dt) {
    u32 old;
    u8 *base;
    s16 w;

    old = a->flags;
    a->flags |= 0x18;
    while (dt > 0) {
        a->flags &= ~2;
        if (a->wait <= dt) {
            w = a->wait;
            a->time += w;
            dt -= w;
            a->wait = 0;
        } else {
            a->wait -= dt;
            a->time += dt;
        }
        if (a->wait > 0)
            break;
        a->prev = a->cur;
        do {
            base = (u8 *)a->cur + ((u32)a->res + a->res->cmdOfs);
        } while (gBACFrameGroupFuncTable[*(u16 *)base](base, a, 0, 0));
        if ((a->flags & 2) && !(a->flags & 4)) {
            a->flags |= 1;
            break;
        }
    }
    a->flags &= ~0x18;
    a->flags |= old & 0x18;
}
