#include <nitro/types.h>

void func_0200bad8(void *f);
void DC_StoreRange(void *start, u32 size);

typedef struct { u8 unk0[8]; u32 unk8; u32 unkC; u8 unk10[8]; u8 unk18[0xC]; u32 unk24; u8 unk28[0x34]; void *unk5C; u32 unk60; } UnkStruct_02033b44;

static inline BOOL func_inline_02033b44(UnkStruct_02033b44 *s) {
    return (BOOL)(s->unk24 & 1 ? 1 : 0);
}

void FileRequest_Finish(UnkStruct_02033b44 *s) {
    if (func_inline_02033b44(s)) return;
    func_0200bad8(s->unk18);
    s->unkC &= ~2;
    s->unk8 = 5;
    DC_StoreRange(s->unk5C, s->unk60);
}
