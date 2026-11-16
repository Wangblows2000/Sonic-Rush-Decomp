#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; } UnkSub_02021100;
typedef struct { u8 unk0[0x18]; u32 unk18; u8 unk1C[8]; UnkSub_02021100 unk24; } UnkStruct_02021100;

void FreeHead(UnkStruct_02021100 *s) {
    UnkSub_02021100 *sub = &s->unk24;
    sub->unk0 = s->unk18;
    sub->unk8 = 0;
}
