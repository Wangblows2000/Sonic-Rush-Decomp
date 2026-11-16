#include <nitro/types.h>

typedef struct {
    s32 v;
    u8 pad4[0x64 - 4];
} UnkField_0233b1d8;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233b1d8;

extern UnkPlayer_0233b1d8 data_ov000_0236aad8[];
extern UnkField_0233b1d8 data_ov000_0236ab14[];
extern UnkField_0233b1d8 data_ov000_0236ab18[];
extern UnkField_0233b1d8 data_ov000_0236ab1c[];
extern UnkField_0233b1d8 data_ov000_0236ab20[];

void func_ov000_0233b1d8(s32 idx, s32 a, s32 b) {
    data_ov000_0236ab1c[idx].v = a;
    data_ov000_0236ab20[idx].v = b;
    if (data_ov000_0236aad8[idx].flags & 0x3000)
        return;
    data_ov000_0236ab14[idx].v = data_ov000_0236ab1c[idx].v;
    data_ov000_0236ab18[idx].v = data_ov000_0236ab20[idx].v;
}
