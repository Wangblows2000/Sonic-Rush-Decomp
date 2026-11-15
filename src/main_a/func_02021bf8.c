#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 unk8; u8 unkC[0xC]; u32 unk18; } UnkStruct_02021bf8;

void func_02021bf8(UnkStruct_02021bf8 *s, u32 a, u32 b) {
    if (a != 0) s->unk8 = a;
    if (b != 0) s->unk18 = b;
}
