#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; } UnkEntry_0201d0f8;
typedef struct { u32 unk0; u32 unk4; u32 count; UnkEntry_0201d0f8 entries[1]; } UnkTable_0201d0f8;
typedef struct { u8 unk0[0x80]; UnkTable_0201d0f8 *unk80; } UnkStruct_0201d0f8;
extern UnkStruct_0201d0f8 *data_02083f04;

u32 NNS_SndArcGetFileAddress(u32 idx) {
    UnkTable_0201d0f8 *t = data_02083f04->unk80;
    if (idx >= t->count) return 0;
    return t->entries[idx].unk8;
}
