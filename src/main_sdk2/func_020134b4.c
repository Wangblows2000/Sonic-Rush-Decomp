#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 pad3[0xd];
    u32 unk10;
    u32 unk14;
    u8 pad18[0x68 - 0x18];
} UnkEntry_020134b4;

BOOL func_02015520(void);
u32 func_02014288(u32 a, u32 b);
u32 func_0201566c(void);
s32 func_02014630(void *buf, u32 bitmap, u8 c, u32 a, u32 b, u32 d, u32 e);

s32 func_020134b4(u8 *ctx, s32 idx, void *buf)
{
    s32 ret = 0;
    UnkEntry_020134b4 *e = (UnkEntry_020134b4 *)(ctx + 0x1d4) + idx;
    u32 bitmap;
    u32 x;

    if (e->state == 2) {
        bitmap = func_02015520() == 0 ? 0xffff : 1;
        if (e->type == 6) {
            x = func_02014288(e->unk10, e->unk14);
            ret = func_02014630(buf, bitmap, e->unk2, e->unk10, e->unk14, x, func_0201566c());
            e->state = 1;
        }
    }
    return ret;
}
