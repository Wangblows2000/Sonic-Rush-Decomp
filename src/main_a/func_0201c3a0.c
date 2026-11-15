#include <nitro/types.h>

void func_02008b38(u32 mask, u32 v);

typedef struct { u8 unk0[0x48]; s32 unk48; u8 unk4C[1]; } UnkStruct_0201c3a0;

void func_0201c3a0(UnkStruct_0201c3a0 *s, s32 idx, u32 v) {
    if (idx > s->unk48 - 1) return;
    func_02008b38(1 << s->unk4C[idx], v);
}
