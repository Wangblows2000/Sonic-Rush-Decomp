#include <nitro/types.h>

typedef struct {
    u8 data[0x68];
} UnkEntry_020130e8;

typedef struct {
    u32 type;
    u8 pad4[0x98];
} UnkPending_020130e8;

typedef struct {
    u32 busy;              /* 0x00 */
    u8 pad4[4];
    u16 bitmap;            /* 0x08 */
    u8 padA[2];
    u8 unkC;               /* 0x0c */
    u8 padD[7];
    u32 unk14;             /* 0x14 */
    u32 unk18;             /* 0x18 */
    u32 unk1c[16];         /* 0x1c */
    u32 unk5c[16];         /* 0x5c */
    UnkPending_020130e8 pending; /* 0x9c */
    u8 pad138[0x1f8 - 0x138];
    UnkEntry_020130e8 entries[16]; /* 0x1f8 */
} UnkCtx_020130e8;

void WBT_IncCommandCounter(void);
u8 WBT_GetCommandCounter(void);
u32 WBT_GetMyDataPacketSize(void);
u32 WBT_GetPeerDataPacketSize(void);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
s32 WBT_MakeReqSyncPacket(void *buf, u16 bitmap, u8 c, u32 a, u32 b);
void WBT_InitPacketBitmap(void *entry, u32 a, void *b, u32 c);
s32 WBT_MakeReqGetBlockPacket(void *buf, u16 bitmap, u8 c, u32 a, u32 b);
s32 WBT_MakeReqGetBlockInfoPacket(void *buf, u16 bitmap, u8 c, u32 a, u32 b);
s32 WBT_MakeReqUserDataPacket(void *buf, u16 bitmap, u8 c, void *a, u8 b);

s32 WBT_StartQueuedCommand(UnkCtx_020130e8 *ctx, void *buf)
{
    s32 ret = 0;
    u8 x;
    s32 i;
    UnkPending_020130e8 *pend = &ctx->pending;

    if (ctx->busy != 0) {
        return ret;
    }
    if (pend->type == 0) {
        return ret;
    }
    WBT_IncCommandCounter();
    x = WBT_GetCommandCounter();
    switch (pend->type) {
    case 2:
        MIi_CpuCopy32(pend, ctx, sizeof(UnkPending_020130e8));
        ctx->unkC = x;
        ret = WBT_MakeReqSyncPacket(buf, ctx->bitmap, ctx->unkC, WBT_GetMyDataPacketSize(), WBT_GetPeerDataPacketSize());
        break;
    case 6:
        MIi_CpuCopy32(pend, ctx, sizeof(UnkPending_020130e8));
        ctx->unkC = x;
        for (i = 0; i < 16; i++) {
            if ((ctx->bitmap >> i) & 1) {
                UnkEntry_020130e8 *e = &ctx->entries[i];
                WBT_InitPacketBitmap(e, ctx->unk18, e->data + 0x1c, ctx->unk1c[i]);
            }
        }
        if (pend->type == 4) {
            ret = WBT_MakeReqGetBlockPacket(buf, ctx->bitmap, ctx->unkC, ctx->unk14, 0);
        } else {
            ret = WBT_MakeReqGetBlockInfoPacket(buf, ctx->bitmap, ctx->unkC, ctx->unk14, 0);
        }
        break;
    case 4:
        MIi_CpuCopy32(pend, ctx, sizeof(UnkPending_020130e8));
        ctx->unkC = x;
        for (i = 0; i < 16; i++) {
            if ((ctx->bitmap >> i) & 1) {
                WBT_InitPacketBitmap(&ctx->entries[i], ctx->unk18, (void *)ctx->unk5c[i], ctx->unk1c[i]);
            }
        }
        if (pend->type == 4) {
            ret = WBT_MakeReqGetBlockPacket(buf, ctx->bitmap, ctx->unkC, ctx->unk14, 0);
        } else {
            ret = WBT_MakeReqGetBlockInfoPacket(buf, ctx->bitmap, ctx->unkC, ctx->unk14, 0);
        }
        break;
    case 10:
        MIi_CpuCopy32(pend, ctx, sizeof(UnkPending_020130e8));
        ctx->unkC = x;
        ret = WBT_MakeReqUserDataPacket(buf, ctx->bitmap, ctx->unkC, &ctx->unk14, *((u8 *)ctx + 0x1d));
        break;
    }
    pend->type = 0;
    return ret;
}
