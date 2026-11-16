#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 pad3[0xd];
    u32 unk10;
    u32 unk14;
    u8 pad18[0x68 - 0x18];
} UnkEntry_020135f8;

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
    u32 unk18;
    u32 unk1c;
    s16 unk20;
} UnkSub_020135f8;

typedef struct {
    u8 pad0[0x32];
    u16 unk32;
} UnkInfo_020135f8;

BOOL func_02015520(void);
UnkInfo_020135f8 *func_02015540(u32 a);
s32 func_02014320(u32 a, u32 b);
u32 func_02014288(u32 a, u32 b);
u32 func_0201566c(void);
s32 func_0201478c(void *buf, u32 bitmap, u8 c, u32 a, u32 b, u32 d, s32 e);
void func_02013f58(u8 *ctx, u32 type, s32 idx, u32 z);

s32 func_020135f8(u8 *ctx, s32 idx, void *buf)
{
    s32 ret = 0;
    UnkSub_020135f8 *sub = (UnkSub_020135f8 *)(ctx + 0x138);
    UnkEntry_020135f8 *e = (UnkEntry_020135f8 *)(ctx + 0x1d4) + idx;
    u32 bitmap;
    UnkInfo_020135f8 *info;
    s32 x;

    if (e->state == 2) {
        bitmap = func_02015520() == 0 ? 0xfffe : 1;
        if (e->type == 4) {
            info = func_02015540(e->unk10);
            if (info != NULL) {
                x = func_02014320(e->unk10, e->unk14);
                if (x != -1) {
                    if (info->unk32 == 0) {
                        u32 y = func_02014288(e->unk10, x);
                        ret = func_0201478c(buf, bitmap, e->unk2, e->unk10, x, y, func_0201566c());
                    } else if (info->unk32 == 1) {
                        sub->unk14 = e->unk10;
                        sub->unk18 = x;
                        sub->unk20 = func_0201566c();
                        sub->unk1c = 0;
                        func_02013f58(ctx, 0xd, idx, 0);
                        if (sub->unk1c != 0) {
                            ret = func_0201478c(buf, bitmap, e->unk2, sub->unk14, sub->unk18, sub->unk1c, sub->unk20);
                        }
                    }
                }
            }
            e->state = 1;
        }
    }
    return ret;
}
