#include <nitro/types.h>

typedef struct {
    u8 data[0x68];
} UnkEntry_02013350;

typedef struct {
    u32 type;              /* 0x00 */
    u8 pad4[4];
    u16 bitmap;            /* 0x08 */
    u8 padA[2];
    u8 unkC;               /* 0x0c */
    u8 padD[7];
    u32 unk14;             /* 0x14 */
    u8 pad18[5];
    u8 unk1d;              /* 0x1d */
    u8 pad1e[0x1f8 - 0x1e];
    UnkEntry_02013350 entries[16]; /* 0x1f8 */
} UnkCtx_02013350;

u32 WBT_Unk_0201566c(void);
u32 WBT_Unk_0201567c(void);
s32 WBT_Unk_02014990(void *buf, u16 bitmap, u8 c, u32 a, u32 b);
s32 WBT_Unk_02014110(void *entry);
s32 WBT_Unk_020145b0(void *buf, u16 bitmap, u8 c, u32 a);
s32 WBT_Unk_02014854(void *buf, u16 bitmap, u8 c, u32 a, s32 b);
s32 WBT_Unk_020146f8(void *buf, u16 bitmap, u8 c, u32 a, s32 b);
s32 WBT_Unk_02014498(void *buf, u16 bitmap, u8 c, void *a, u8 b);

s32 WBT_Unk_02013350(UnkCtx_02013350 *ctx, void *buf)
{
    s32 ret = 0;
    s32 i;

    switch (ctx->type) {
    case 2:
        return WBT_Unk_02014990(buf, ctx->bitmap, ctx->unkC, WBT_Unk_0201566c(), WBT_Unk_0201567c());
    case 4:
    case 6:
        for (i = 0; i < 16; i++) {
            if (ctx->bitmap & (u16)(1 << i)) {
                s32 r = WBT_Unk_02014110(&ctx->entries[i]);
                if (r == -1) {
                    ret = WBT_Unk_020145b0(buf, 1 << i, ctx->unkC, ctx->unk14);
                } else {
                    u16 bm = ctx->bitmap;
                    if (ctx->type == 4) {
                        ret = WBT_Unk_02014854(buf, bm, ctx->unkC, ctx->unk14, r);
                    } else {
                        ret = WBT_Unk_020146f8(buf, bm, ctx->unkC, ctx->unk14, r);
                    }
                }
                if (ret != 0) {
                    return ret;
                }
            }
        }
        break;
    case 10:
        return WBT_Unk_02014498(buf, ctx->bitmap, ctx->unkC, &ctx->unk14, ctx->unk1d);
    }
    return ret;
}
