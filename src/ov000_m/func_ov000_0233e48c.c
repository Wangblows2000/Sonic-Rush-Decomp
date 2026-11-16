#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4[2];
    s32 unk8[2];
} UnkStruct_0233e48c;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233e48c;

extern UnkPlayer_0233e48c data_ov000_0236aad8[];
extern UnkStruct_0233e48c *data_ov000_0236ac18;
void func_ov000_0233c540(UnkStruct_0233e48c *w, u8 i);

void func_ov000_0233e48c(u32 mask) {
    UnkStruct_0233e48c *w;
    u8 i;

    if (!(data_ov000_0236aad8[2].flags & 0x100000))
        return;
    w = data_ov000_0236ac18;
    if (w == NULL)
        return;
    for (i = 0; i < 2; i++) {
        if (mask & (1 << i)) {
            func_ov000_0233c540(w, i);
            w->unk4[i] = 0x3c;
            w->unk8[i] = 3;
        }
    }
}
