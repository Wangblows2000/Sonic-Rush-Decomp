#include <nitro/types.h>

typedef struct UnkStruct_0233b158 {
    u32 unk0;
    u8 pad[0x60];
} UnkStruct_0233b158;

extern UnkStruct_0233b158 gCamera[];

void func_ov000_0233b158(s32 i) {
    gCamera[i].unk0 &= ~0x80;
}
