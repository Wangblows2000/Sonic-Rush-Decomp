#include <nitro/types.h>

typedef struct UnkStruct_022f8810_2 {
    u8 pad[0x458];
    s32 unk458;
} UnkStruct_022f8810_2;

typedef struct UnkStruct_022f8920 {
    u8 pad[0x218];
    UnkStruct_022f8810_2 *unk218;
} UnkStruct_022f8920;

extern UnkStruct_022f8920 *data_ov000_02369b84;

void func_ov000_022f345c(UnkStruct_022f8810_2 *p);

void func_ov000_022f8920(s32 a) {
    data_ov000_02369b84->unk218->unk458 = a;
    func_ov000_022f345c(data_ov000_02369b84->unk218);
}
