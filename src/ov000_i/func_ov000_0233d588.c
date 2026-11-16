#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x20 - 0xc];
    s16 unk20;
    u8 pad22[0x62 - 0x22];
    u16 unk62;
} UnkPlayer_0233d588;

typedef struct {
    u8 pad0[4];
    u8 state[4];
    s8 ratio[4];
} UnkStruct_0233d588;

extern UnkPlayer_0233d588 data_ov000_0236aad8[2];
s32 func_0200225c(s32 a, s32 b);

void func_ov000_0233d588(UnkStruct_0233d588 *w) {
    UnkPlayer_0233d588 *p;
    s32 top;
    u8 i;

    for (i = 0; i < 2; i++) {
        p = &data_ov000_0236aad8[i];
        if (!(p->flags & 0x1000000) || (p->flags & 0x2000000)) {
            w->state[i] = 3;
        } else {
            top = p->unk62 << 8;
            if (p->unk8 < top - p->unk20 * 0xc0) {
                w->state[i] = 0;
            } else if (p->unk8 < top) {
                w->state[i] = 1;
                w->ratio[i] = func_0200225c((p->unk62 << 8) - p->unk8, p->unk20) >> 12;
            } else {
                w->state[i] = 2;
            }
        }
    }
}
