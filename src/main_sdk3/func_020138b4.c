#include <nitro/types.h>

typedef struct {
    u8 state;     /* 0x00 */
    u8 type;      /* 0x01 */
    u8 unk2;      /* 0x02 */
    u8 pad3;
    s16 unk4;     /* 0x04 */
    s16 unk6;     /* 0x06 */
    u16 unk8;     /* 0x08 */
    s16 unkA;     /* 0x0a */
    u8 padC;
    u8 unkD;      /* 0x0d */
    u8 padE[2];
    u32 unk10;    /* 0x10 */
    u32 unk14;    /* 0x14 */
    u8 pad18[8];
    u8 *unk20;    /* 0x20 */
    u8 unk24[0x68 - 0x24];
} UnkEntry_020138b4;

typedef struct {
    u32 busy;     /* 0x00 */
    u32 result;   /* 0x04 */
    u16 bitmap;   /* 0x08 */
    u8 padA[0xa];
    u32 unk14;    /* 0x14 */
} UnkCtx_020138b4;

void MI_CpuCopy8(const void *src, void *dest, u32 size);
s32 func_02015520(void);
void func_0201564c(s32 a);
void func_0201565c(s32 a);
void func_02013e7c(UnkCtx_020138b4 *ctx, u32 type, s32 idx);
void func_02013f58(UnkCtx_020138b4 *ctx, s32 type, s32 idx, u16 x);
BOOL func_020141d8(void *info, u32 a);
BOOL func_02014078(void *info, u32 a, const void *src);
void func_020141a8(void *info, u32 a);

static inline BOOL func_020138b4_IsMine(u16 bm)
{
    return func_02015520() == -1 ? FALSE : (bm & (1 << func_02015520())) ? TRUE : FALSE;
}

s32 func_020138b4(UnkCtx_020138b4 *ctx, s32 idx, u8 *buf, s32 len)
{
    s32 n = 0;
    s32 state = 1;
    UnkEntry_020138b4 *e = (UnkEntry_020138b4 *)((u8 *)ctx + 0x1d4) + idx;
    void *info = (u8 *)ctx + 0x1f8 + idx * 0x68;

    e->state = 1;
    if (len == 0) {
        return n;
    }
    while (n < len) {
        switch (state) {
        case 1:
            MI_CpuCopy8(buf, &e->type, 1);
            buf += 1;
            n += 1;
            state = 2;
            break;
        case 2:
            MI_CpuCopy8(buf, &e->unk8, 2);
            buf += 2;
            n += 2;
            if (!func_020138b4_IsMine(e->unk8)) {
                return n;
            }
            state = 3;
            break;
        case 3:
            MI_CpuCopy8(buf, &e->unk2, 1);
            buf += 1;
            n += 1;
            switch (e->type) {
            case 1:
                return n;
            case 2:
                state = 5;
                break;
            case 3:
                state = 4;
                break;
            case 11:
                if (ctx->busy == 10) {
                    func_02013e7c(ctx, e->type, idx);
                }
                return n;
            case 10:
                state = 10;
                break;
            case 0:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            default:
                state = 7;
                break;
            }
            break;
        case 10:
            MI_CpuCopy8(buf, &e->unkD, 1);
            buf += 1;
            n += 1;
            state = 9;
            break;
        case 9:
            e->unk20 = buf;
            func_02013f58(ctx, e->type, idx, 0);
            e->state = 2;
            return n + 9;
        case 4:
            MI_CpuCopy8(buf, &e->unkA, 2);
            buf += 2;
            n += 2;
            if (e->type == 3) {
                state = 5;
            }
            break;
        case 5:
            MI_CpuCopy8(buf, &e->unk6, 2);
            MI_CpuCopy8(buf + 2, &e->unk4, 2);
            if (func_02015520()) {
                func_0201564c(e->unk4);
                func_0201565c(e->unk6);
            }
            switch (e->type) {
            case 2:
                func_02013f58(ctx, e->type, idx, 0);
                e->state = 2;
                return n + 4;
            case 3:
                if (ctx->busy == 2) {
                    func_02013e7c(ctx, e->type, idx);
                }
                return n + 4;
            default:
                func_02013f58(ctx, 0xf, idx, 3);
                return n + 4;
            }
        case 7:
            MI_CpuCopy8(buf, &e->unk10, 4);
            buf += 4;
            n += 4;
            switch (e->type) {
            case 8:
                func_02013f58(ctx, e->type, idx, 0);
                e->state = 2;
                return n;
            case 9:
                if (ctx->busy == 4) {
                    if (ctx->unk14 == e->unk10) {
                        func_02013e7c(ctx, 5, idx);
                    }
                } else if (ctx->busy == 6) {
                    if (ctx->unk14 == e->unk10) {
                        func_02013e7c(ctx, 7, idx);
                    }
                }
                return n;
            case 4:
            case 5:
            case 6:
            case 7:
                state = 8;
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            default:
                func_02013f58(ctx, 0xf, idx, 3);
                break;
            }
            break;
        case 8:
            MI_CpuCopy8(buf, &e->unk14, 4);
            buf += 4;
            n += 4;
            switch (e->type) {
            case 4:
            case 6:
                e->state = 2;
                return n;
            case 5:
            case 7:
                e->unk20 = buf;
                if (ctx->busy == 4 || ctx->busy == 6) {
                    if (e->unk10 == ctx->unk14 && (ctx->bitmap & (u16)(1 << idx))) {
                        if (!func_020141d8(info, e->unk14)) {
                            if (func_02014078(info, e->unk14, e->unk20) == TRUE) {
                                func_020141a8(info, e->unk14);
                            } else {
                                func_02013f58(ctx, 0xf, idx, 6);
                            }
                        }
                    }
                }
                return n;
            case 0:
            case 1:
            case 2:
            case 3:
            default:
                func_02013f58(ctx, 0xf, idx, 3);
                break;
            }
            break;
        default:
            func_02013f58(ctx, 0xf, idx, 4);
            break;
        }
    }
    func_02013f58(ctx, 0xf, idx, 5);
    return n;
}
