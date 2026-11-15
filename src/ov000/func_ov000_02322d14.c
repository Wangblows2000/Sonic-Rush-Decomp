#include <nitro/types.h>

typedef struct UnkStruct_02322d14_2 {
    u8 pad[8];
    s32 unk8;
} UnkStruct_02322d14_2;

typedef struct UnkStruct_02322d14 {
    u8 pad[0x10];
    UnkStruct_02322d14_2 *unk10;
} UnkStruct_02322d14;

extern UnkStruct_02322d14 *data_ov000_0236a660[];

void func_0206bef4(UnkStruct_02322d14 *s);

void func_ov000_02322d14(UnkStruct_02322d14 *s) {
    s32 i = s->unk10->unk8;
    if (s == data_ov000_0236a660[i])
        data_ov000_0236a660[i] = NULL;
    func_0206bef4(s);
}
