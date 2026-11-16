#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 dataOfs; } UnkRes_020322bc;
typedef struct UnkCtx_020322bc {
    u8 unk0[0xE];
    s16 frame;
    UnkRes_020322bc *res;
    u8 unk14[8];
    u32 pos;
    u32 prevPos;
    u8 unk24[4];
    s16 wait;
    u8 unk2a[2];
    u32 flags;
} UnkCtx_020322bc;

typedef BOOL (*UnkCmd_020322bc)(void *cmd, UnkCtx_020322bc *c, u32 a, u32 b);
extern UnkCmd_020322bc data_02072e00[];

static inline u32 GetData_020322bc(UnkRes_020322bc *r) {
    return (u32)r + r->dataOfs;
}

void func_020322bc(UnkCtx_020322bc *c, s16 frames, u32 a, u32 b) {
    u32 saved = c->flags;
    s16 w;
    c->flags |= 0x18;
    while (frames > 0) {
        c->flags &= ~2;
        w = c->wait;
        if (w <= frames) {
            c->frame += w;
            frames -= w;
            c->wait = 0;
        } else {
            c->wait = w - frames;
            c->frame += frames;
        }
        if (c->wait > 0) {
            break;
        }
        c->prevPos = c->pos;
        while (data_02072e00[*(u16 *)(c->pos + GetData_020322bc(c->res))]((void *)(c->pos + GetData_020322bc(c->res)), c, a, b)) {
        }
        {
            u32 f = c->flags;
            if ((f & 2) && !(f & 4)) {
                c->flags = f | 1;
                break;
            }
        }
    }
    c->flags &= ~0x18;
    c->flags |= saved & 0x18;
}
