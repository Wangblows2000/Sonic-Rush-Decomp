#include <nitro/types.h>

typedef struct { u32 count; u32 offsets[1]; } UnkTable_0201d214;
typedef struct { u8 unk0[0x1C]; u32 tableOffset; } UnkArc_0201d214;
typedef struct { u8 unk0[0x88]; UnkArc_0201d214 *unk88; } UnkStruct_0201d214;
extern UnkStruct_0201d214 *data_02083f04;

static inline void *OffsetToPtr_0201d214(void *base, u32 off) {
    return off == 0 ? NULL : (u8 *)base + off;
}

void *func_0201d214(s32 idx) {
    UnkStruct_0201d214 *g = data_02083f04;
    UnkTable_0201d214 *t = OffsetToPtr_0201d214(g->unk88, g->unk88->tableOffset);
    if (t == NULL) return NULL;
    if (idx < 0) return NULL;
    if (idx >= t->count) return NULL;
    return OffsetToPtr_0201d214(g->unk88, t->offsets[idx]);
}
