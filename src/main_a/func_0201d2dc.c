#include <nitro/types.h>

typedef struct { u32 count; u32 offsets[1]; } UnkTable_0201d2dc;
typedef struct { u8 unk0[0x18]; u32 tableOffset; } UnkArc_0201d2dc;
typedef struct { u8 unk0[0x88]; UnkArc_0201d2dc *unk88; } UnkStruct_0201d2dc;
extern UnkStruct_0201d2dc *data_02083f04;

static inline void *OffsetToPtr_0201d2dc(void *base, u32 off) {
    return off == 0 ? NULL : (u8 *)base + off;
}

void *func_0201d2dc(s32 idx) {
    UnkStruct_0201d2dc *g = data_02083f04;
    UnkTable_0201d2dc *t = OffsetToPtr_0201d2dc(g->unk88, g->unk88->tableOffset);
    if (t == NULL) return NULL;
    if (idx < 0) return NULL;
    if (idx >= t->count) return NULL;
    return OffsetToPtr_0201d2dc(g->unk88, t->offsets[idx]);
}
