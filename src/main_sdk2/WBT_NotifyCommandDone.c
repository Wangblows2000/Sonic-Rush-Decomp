#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 pad3;
    s16 unk4;
    s16 unk6;
    u8 pad8[2];
    s16 unkA;
    u8 padC[0x68 - 0xc];
} UnkEntry_02013e7c;

typedef struct UnkCtx_02013e7c {
    u32 busy;      /* 0x00 */
    u32 result;    /* 0x04 */
    u16 bitmap;    /* 0x08 */
    u16 lastBit;   /* 0x0a */
    u8 unkC;       /* 0x0c */
    u8 unkD;       /* 0x0d */
    u8 padE[2];
    void (*cb)(struct UnkCtx_02013e7c *); /* 0x10 */
    s16 unk14;
    s16 unk16;
    s16 unk18;
} UnkCtx_02013e7c;

void WBT_NotifyCommandDone(UnkCtx_02013e7c *ctx, u32 type, s32 idx)
{
    UnkEntry_02013e7c *e = (UnkEntry_02013e7c *)((u8 *)ctx + 0x1d4) + idx;
    u16 bit = 1 << idx;
    ctx->lastBit = bit;
    if (ctx->unkC != e->unk2) {
        return;
    }
    ctx->unkD = e->unk2;
    if (ctx->bitmap & bit) {
        ctx->bitmap &= ~bit;
        switch (type) {
        case 0:
        case 1:
        case 2:
            break;
        case 3:
            ctx->unk14 = e->unkA;
            ctx->unk16 = e->unk6;
            ctx->unk18 = e->unk4;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            break;
        }
        ctx->result = type;
        if (ctx->cb != NULL) {
            ctx->cb(ctx);
        }
    }
    if (ctx->bitmap == 0) {
        ctx->busy = 0;
    }
}
