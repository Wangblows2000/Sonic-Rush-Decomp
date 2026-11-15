#include <nitro/types.h>

typedef struct UnkStruct_023121e4_2 {
    u8 unk0;
    u8 pad[0x61];
    u16 unk62;
} UnkStruct_023121e4_2;

typedef struct UnkStruct_023121e4 {
    u8 pad[0x18];
    UnkStruct_023121e4_2 *unk18;
} UnkStruct_023121e4;

void func_ov000_023121e4(UnkStruct_023121e4 *a, UnkStruct_023121e4 *b) {
    UnkStruct_023121e4_2 *pa = a->unk18;
    UnkStruct_023121e4_2 *pb = b->unk18;
    if (pa == NULL)
        return;
    if (pb == NULL && pb->unk0 != 1)
        return;
    pb->unk62 = 0xf;
}
