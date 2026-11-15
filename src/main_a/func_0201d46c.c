#include <nitro/types.h>

typedef struct { u32 count; u32 offsets[1]; } UnkTable_0201d46c;
typedef struct { u8 unk0[0xC]; u32 tableOffset; } UnkArc_0201d46c;
typedef struct { u8 unk0[0x88]; UnkArc_0201d46c *unk88; } UnkStruct_0201d46c;
extern UnkStruct_0201d46c *data_02083f04;

static inline void *OffsetToPtr_0201d46c(void *base, u32 off) {
    return off == 0 ? NULL : (u8 *)base + off;
}

void *func_0201d46c(s32 idx) {
    UnkStruct_0201d46c *g = data_02083f04;
    UnkTable_0201d46c *t = OffsetToPtr_0201d46c(g->unk88, g->unk88->tableOffset);
    if (t == NULL) return NULL;
    if (idx < 0) return NULL;
    if (idx >= t->count) return NULL;
    return OffsetToPtr_0201d46c(g->unk88, t->offsets[idx]);
}
