#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x34 - 4];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_023423b8;

extern UnkPlayer_023423b8 data_ov000_0236aad8[];

void func_ov000_023423b8(void) {
    UnkPlayer_023423b8 *p;
    u8 i;

    if (data_ov000_0236aad8[2].flags & 1) {
        p = data_ov000_0236aad8;
        if (!(p->unk34 & 0x40) && (p->unk34 & 0x20))
            p->unk34 |= 0x40;
        else if ((p->unk34 & 0x40) && !(p->unk34 & 0x20))
            p->unk34 &= ~0x40;
        p->unk34 &= ~0x20;
        return;
    }
    for (i = 0; i < 2; i++) {
        if (!(data_ov000_0236aad8[i].unk34 & 0x40) && (data_ov000_0236aad8[i].unk34 & 0x20))
            data_ov000_0236aad8[i].unk34 |= 0x40;
        else if ((data_ov000_0236aad8[i].unk34 & 0x40) && !(data_ov000_0236aad8[i].unk34 & 0x20))
            data_ov000_0236aad8[i].unk34 &= ~0x40;
        data_ov000_0236aad8[i].unk34 &= ~0x20;
    }
}
