#include <nitro/types.h>

typedef struct UnkStruct_023099c0_2 {
    u8 pad[0x214];
    u8 unk214;
} UnkStruct_023099c0_2;

typedef struct UnkStruct_023099c0 {
    u8 pad[0x10];
    UnkStruct_023099c0_2 *unk10;
} UnkStruct_023099c0;

BOOL func_ov000_0234238c(s32 a, s32 i);
void func_ov000_022fb9c8(UnkStruct_023099c0 *s);

void func_ov000_023099c0(UnkStruct_023099c0 *s) {
    func_ov000_0234238c(1, s->unk10->unk214);
    func_ov000_022fb9c8(s);
}
