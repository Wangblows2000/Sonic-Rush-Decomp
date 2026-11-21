#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 pad3[0xd];
    u32 unk10;
    u32 unk14;
    u8 pad18[0x68 - 0x18];
} UnkEntry_02013748;

s32 WBT_MakeResUserDataPacket(void *buf, u16 bitmap, u8 c);

s32 WBT_RespondUserData(u8 *ctx, s32 idx, void *buf)
{
    UnkEntry_02013748 *e = (UnkEntry_02013748 *)(ctx + 0x1d4) + idx;
    u16 bit = 1 << idx;
    s32 ret = 0;

    if (e->state == 2 && e->type == 10) {
        ret = WBT_MakeResUserDataPacket(buf, bit, e->unk2);
        e->state = 1;
    }
    return ret;
}
