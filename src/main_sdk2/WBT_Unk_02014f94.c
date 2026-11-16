#include <nitro/types.h>

extern s32 data_020779ac;

u8 *WBT_Unk_02015640(void);
s32 WBT_Unk_02013748(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_02013814(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_02013554(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_020137ac(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_020135f8(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_020134b4(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_02013350(u8 *ctx, u8 *buf, u32 size);
s32 WBT_Unk_020130e8(u8 *ctx, u8 *buf, u32 size);
s32 WBT_Unk_02014a24(u8 *buf, u16 bitmap, u32 c);

s32 WBT_Unk_02014f94(u8 *buf, u32 size)
{
    u8 *ctx = WBT_Unk_02015640();
    s32 r;
    s32 i = data_020779ac;
    do {
        if (++i > 15) {
            i = 1;
        }
        if ((r = WBT_Unk_02013748(ctx, i, buf, size)) != 0) {
            data_020779ac = i;
            return r;
        }
        if ((r = WBT_Unk_02013814(ctx, i, buf, size)) != 0) {
            data_020779ac = i;
            return r;
        }
        if ((r = WBT_Unk_02013554(ctx, i, buf, size)) != 0) {
            data_020779ac = i;
            return r;
        }
        if ((r = WBT_Unk_020137ac(ctx, i, buf, size)) != 0) {
            data_020779ac = i;
            return r;
        }
    } while (i != data_020779ac);
    if ((r = WBT_Unk_02013350(ctx, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_Unk_020130e8(ctx, buf, size)) != 0) {
        return r;
    }
    i = data_020779ac;
    do {
        if (++i > 15) {
            i = 1;
        }
        if ((r = WBT_Unk_020135f8(ctx, i, buf, size)) != 0) {
            data_020779ac = i;
            return r;
        }
    } while (i != data_020779ac);
    for (i = 1; i < 16; i++) {
        if ((r = WBT_Unk_020134b4(ctx, i, buf, size)) != 0) {
            return r;
        }
    }
    return WBT_Unk_02014a24(buf, 0xfffe, 0);
}
