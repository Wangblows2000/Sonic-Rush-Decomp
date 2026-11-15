#include <nitro/types.h>

typedef struct UnkStruct_02302a14_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_02302a14_2;

typedef struct UnkStruct_02302a14 {
    u8 pad[0x18];
    UnkStruct_02302a14_2 *unk18;
} UnkStruct_02302a14;

void func_ov000_02302a14(UnkStruct_02302a14 *s) {
    s->unk18->unk20C |= 1;
}
