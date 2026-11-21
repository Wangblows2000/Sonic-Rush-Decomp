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

BOOL WBT_GetOwnAid(void);
UnkInfo_020135f8 *WBT_FindBlockById(u32 a);
s32 WBT_PickBlockPacketIndex(u32 a, u32 b);
u32 WBT_GetBlockPacketData(u32 a, u32 b);
u32 WBT_GetMyDataPacketSize(void);
s32 WBT_MakeResGetBlockPacket(void *buf, u32 bitmap, u8 c, u32 a, u32 b, u32 d, s32 e);
void WBT_NotifySystemCallback(u8 *ctx, u32 type, s32 idx, u32 z);

s32 WBT_RespondGetBlock(u8 *ctx, s32 idx, void *buf)
{
    s32 ret = 0;
    UnkSub_020135f8 *sub = (UnkSub_020135f8 *)(ctx + 0x138);
    UnkEntry_020135f8 *e = (UnkEntry_020135f8 *)(ctx + 0x1d4) + idx;
    u32 bitmap;
    UnkInfo_020135f8 *info;
    s32 x;

    if (e->state == 2) {
        bitmap = WBT_GetOwnAid() == 0 ? 0xfffe : 1;
        if (e->type == 4) {
            info = WBT_FindBlockById(e->unk10);
            if (info != NULL) {
                x = WBT_PickBlockPacketIndex(e->unk10, e->unk14);
                if (x != -1) {
                    if (info->unk32 == 0) {
                        u32 y = WBT_GetBlockPacketData(e->unk10, x);
                        ret = WBT_MakeResGetBlockPacket(buf, bitmap, e->unk2, e->unk10, x, y, WBT_GetMyDataPacketSize());
                    } else if (info->unk32 == 1) {
                        sub->unk14 = e->unk10;
                        sub->unk18 = x;
                        sub->unk20 = WBT_GetMyDataPacketSize();
                        sub->unk1c = 0;
                        WBT_NotifySystemCallback(ctx, 0xd, idx, 0);
                        if (sub->unk1c != 0) {
                            ret = WBT_MakeResGetBlockPacket(buf, bitmap, e->unk2, sub->unk14, sub->unk18, sub->unk1c, sub->unk20);
                        }
                    }
                }
            }
            e->state = 1;
        }
    }
    return ret;
}
