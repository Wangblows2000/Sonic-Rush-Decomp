#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; } UnkEntry_0201d0dc;
typedef struct { u8 unk0[0xC]; UnkEntry_0201d0dc entries[1]; } UnkTable_0201d0dc;
typedef struct { u8 unk0[0x80]; UnkTable_0201d0dc *unk80; } UnkStruct_0201d0dc;
extern UnkStruct_0201d0dc *data_02083f04;

void NNS_SndArcSetFileAddress(u32 idx, u32 v) {
    data_02083f04->unk80->entries[idx].unk8 = v;
}
