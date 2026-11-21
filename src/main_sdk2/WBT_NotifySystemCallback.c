#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
    u8 pad8[2];
    s16 unkA;
    u8 padC;
    u8 unkD;
    u8 padE[2];
    u32 unk10;
    u8 pad14[0xc];
    void *unk20;
    u8 pad24[0x68 - 0x24];
} UnkEntry_02013f58;

typedef struct UnkSub_02013f58 {
    u32 unk0;     /* 0x00 */
    s32 type;     /* 0x04 */
    u8 pad8[2];
    u16 bit;      /* 0x0a */
    u8 padC;
    u8 unkD;      /* 0x0d */
    u16 unkE;     /* 0x0e */
    void (*cb)(struct UnkSub_02013f58 *); /* 0x10 */
    union {
        u32 u32v;
        struct {
            s16 a;
            s16 b;
            s16 c;
        } s;
        u8 bytes[9];
    } data;       /* 0x14 */
} UnkSub_02013f58;

void MI_CpuCopy8(const void *src, void *dest, u32 size);

void WBT_NotifySystemCallback(u8 *ctx, s32 type, s32 idx, u16 x)
{
    UnkEntry_02013f58 *e = (UnkEntry_02013f58 *)(ctx + 0x1d4) + idx;
    UnkSub_02013f58 *sub = (UnkSub_02013f58 *)(ctx + 0x138);

    if (type != 13 && e->unk2 == e->unk3) {
        return;
    }
    sub->unk0 = 12;
    sub->bit = 1 << idx;
    e->unk3 = e->unk2;
    sub->unkD = e->unk2;
    sub->unkE = x;
    switch (type) {
    case 8:
        sub->data.u32v = e->unk10;
        break;
    case 2:
        sub->data.s.a = e->unkA;
        sub->data.s.b = e->unk6;
        sub->data.s.c = e->unk4;
        break;
    case 10:
        if (e->unkD > 9) {
            *((u8 *)sub + 0x1d) = 0;
            break;
        }
        *((u8 *)sub + 0x1d) = e->unkD;
        MI_CpuCopy8(e->unk20, &sub->data, *((u8 *)sub + 0x1d));
        break;
    case 11:
    case 14:
        break;
    }
    sub->type = type;
    if (sub->cb != NULL) {
        sub->cb(sub);
    }
}
