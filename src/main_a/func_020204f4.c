#include <nitro/types.h>

typedef struct { s32 unk0; u32 unk4; u32 unk8; } UnkEntry_020204f4;
typedef struct { u8 unk0[0x1C]; u32 count; UnkEntry_020204f4 entries[1]; } UnkStruct_020204f4;

UnkEntry_020204f4 *func_020204f4(UnkStruct_020204f4 *s, s32 idx) {
    if (idx < 0) return NULL;
    if (idx >= s->count) return NULL;
    if (s->entries[idx].unk0 == -1) return NULL;
    return &s->entries[idx];
}
