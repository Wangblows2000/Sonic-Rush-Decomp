#include <nitro/types.h>

/* Bounds-checked offset table lookup. Matched (CW 1.2). Struct layout inferred. */
typedef struct { u32 offset; u32 unk4; } UnkEntry_02021264;
typedef struct { u8 unk0[8]; u16 count; u16 unkA; UnkEntry_02021264 entries[1]; } UnkTable_02021264;
typedef struct { u8 unk0[0x54]; UnkTable_02021264 *table; u32 base; } UnkStruct_02021264;

u32 func_02021264(UnkStruct_02021264 *s, u32 idx) {
    UnkTable_02021264 *t = s->table;
    u32 r = 0;
    if (idx < t->count) {
        r = s->base + t->entries[idx].offset;
    }
    return r;
}
