#include <nitro/types.h>

void func_02034a40(u32 a, u32 b, u32 c);

typedef struct { u32 unk0; u32 *unk4; u32 unk8; } UnkStruct_020349a0;

void func_020349a0(UnkStruct_020349a0 *s, u32 b) {
    func_02034a40(*s->unk4, b, s->unk8);
}
