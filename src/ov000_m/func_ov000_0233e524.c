#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4[2];
    s32 unk8[2];
} UnkStruct_0233e524;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233e524;

extern UnkPlayer_0233e524 data_ov000_0236aad8[];
extern UnkStruct_0233e524 *data_ov000_0236ac18;
void func_ov000_0233c540(UnkStruct_0233e524 *w, u8 i);

void func_ov000_0233e524(u32 mask, s32 n) {
    UnkStruct_0233e524 *w;
    u8 i;
    u16 t;

    if (!(data_ov000_0236aad8[2].flags & 0x100000))
        return;
    w = data_ov000_0236ac18;
    if (w == NULL)
        return;
    if (n <= 0)
        n = 1;
    t = n;
    for (i = 0; i < 2; i++) {
        if (mask & (1 << i)) {
            func_ov000_0233c540(w, i);
            w->unk4[i] = t;
            w->unk8[i] = 2;
        }
    }
}
