#include <nitro/types.h>

typedef struct UnkStruct_02338924_2 {
    u8 pad[0x20];
    s16 unk20;
    u8 pad1[0xc8 - 0x22];
    u32 unkC8;
} UnkStruct_02338924_2;

typedef struct UnkStruct_02338924 {
    u8 pad[8];
    s32 unk8;
} UnkStruct_02338924;

extern UnkStruct_02338924_2 gCamera;

void func_ov000_02338924(UnkStruct_02338924 *s) {
    gCamera.unkC8 |= 0x20000;
    s->unk8 = (gCamera.unk20 << 12) >> 8;
}
