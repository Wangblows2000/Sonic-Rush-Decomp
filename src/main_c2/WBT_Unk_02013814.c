#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 pad3[0xd];
    u32 unk14;
    u8 pad18[0x68 - 0x18];
    u32 unk10;
} UnkEntry_02013748;

s32 WBT_NumOfRegisteredBlock(void);
u32 WBT_Unk_0201566c(void);
u32 WBT_Unk_0201567c(void);
s32 WBT_Unk_020148e8(void *buf, u16 bitmap, u8 c, s16 d, u32 e, u32 f);

s32 WBT_Unk_02013814(u8 *ctx, s32 idx, void *buf)
{
    s32 ret = 0;
    UnkEntry_02013748 *e = (UnkEntry_02013748 *)(ctx + 0x1d4) + idx;
    u16 bit = 1 << idx;

    if (e->state == 2 && e->type == 2) {
        ret = WBT_Unk_020148e8(buf, bit, e->unk2, (s16)WBT_NumOfRegisteredBlock(), WBT_Unk_0201566c(), WBT_Unk_0201567c());
        e->state = 1;
    }
    return ret;
}
