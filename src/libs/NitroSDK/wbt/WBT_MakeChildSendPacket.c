#include <nitro/types.h>

u8 *WBT_GetWork(void);
s32 WBT_RespondUserData(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_RespondSync(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_RespondGetBlockInfo(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_RespondGetBlockDone(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_RespondGetBlock(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_Unk_020134b4(u8 *ctx, s32 idx, u8 *buf, u32 size);
s32 WBT_MakeCurrentCommandPacket(u8 *ctx, u8 *buf, u32 size);
s32 WBT_StartQueuedCommand(u8 *ctx, u8 *buf, u32 size);
s32 WBT_MakeWaitPacket(u8 *buf, u16 bitmap, u32 c);

s32 WBT_MakeChildSendPacket(u8 *buf, u32 size)
{
    u8 *ctx = WBT_GetWork();
    s32 r;
    if ((r = WBT_RespondUserData(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_RespondSync(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_RespondGetBlockInfo(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_RespondGetBlockDone(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_RespondGetBlock(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_Unk_020134b4(ctx, 0, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_MakeCurrentCommandPacket(ctx, buf, size)) != 0) {
        return r;
    }
    if ((r = WBT_StartQueuedCommand(ctx, buf, size)) != 0) {
        return r;
    }
    return WBT_MakeWaitPacket(buf, 0xffff, 0);
}
