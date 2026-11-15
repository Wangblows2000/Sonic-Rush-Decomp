#include <nitro/types.h>

typedef struct UnkStruct_02320894_2 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_02320894_2;

typedef struct UnkStruct_02320894 {
    u8 pad[0x18];
    UnkStruct_02320894_2 *unk18;
} UnkStruct_02320894;

void func_ov000_02320894(UnkStruct_02320894 *s) {
    s->unk18->unkC = 1;
}
