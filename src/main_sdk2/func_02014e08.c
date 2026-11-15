#include <nitro/types.h>

u8 *func_02015640(void);
s32 func_02013748(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 func_02013814(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 func_02013554(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 func_020137ac(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 func_020135f8(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 func_020134b4(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 func_02013350(u8 *ctx, u8 *buf, u32 size);
s32 func_020130e8(u8 *ctx, u8 *buf, u32 size);
s32 func_02014a24(u8 *buf, u16 bitmap, u32 c);

s32 func_02014e08(u8 *buf, u32 size)
{
    u8 *ctx = func_02015640();
    s32 r;
    if ((r = func_02013748(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = func_02013814(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = func_02013554(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = func_020137ac(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = func_020135f8(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = func_020134b4(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = func_02013350(ctx, buf, size)) != 0) {
        return r;
    }
    if ((r = func_020130e8(ctx, buf, size)) != 0) {
        return r;
    }
    return func_02014a24(buf, 0xffff, 0);
}
