#include <nitro/types.h>

typedef struct {
    u32 unk0;
    s32 a[2];
    s32 b[2];
    u8 pad14[0x1E - 0x14];
    s16 d[2];
    u16 e;
    u8 pad24[0x2C - 0x24];
    s32 c[2];
    u8 pad34[0xC8 - 0x34];
    u32 flags;
} UnkStruct_0233b470;

extern UnkStruct_0233b470 data_ov000_0236aad8;
extern UnkStruct_0233b470 data_ov000_0236ab3c;

#define XSWAP_0233b470(x, y) \
    do {                     \
        (x) ^= (y);          \
        (y) ^= (x);          \
        (x) ^= (y);          \
    } while (0)

BOOL func_ov000_0233b470(BOOL on) {
    UnkStruct_0233b470 *p;
    UnkStruct_0233b470 *q;
    u8 i;

    if (!on) {
        if (!(data_ov000_0236aad8.flags & 2)) {
            return FALSE;
        }
        data_ov000_0236aad8.flags |= 0x200;
        data_ov000_0236aad8.flags &= ~2;
        data_ov000_0236aad8.flags &= ~0x400;
    } else {
        if (data_ov000_0236aad8.flags & 2) {
            return FALSE;
        }
        data_ov000_0236aad8.flags |= 0x200;
        data_ov000_0236aad8.flags |= 2;
        data_ov000_0236aad8.flags |= 0x400;
    }
    p = &data_ov000_0236aad8;
    q = &data_ov000_0236ab3c;
    for (i = 0; i < 2; i++) {
        XSWAP_0233b470(p->a[i], q->a[i]);
        XSWAP_0233b470(p->b[i], q->b[i]);
        XSWAP_0233b470(p->c[i], q->c[i]);
        XSWAP_0233b470(p->d[i], q->d[i]);
    }
    XSWAP_0233b470(p->e, q->e);
    return TRUE;
}
