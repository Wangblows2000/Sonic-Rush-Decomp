#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; s32 unk4; u8 unk8[0x18]; } UnkEntry;
typedef struct { u32 unk0; u32 count; u32 offset; } UnkTable;
typedef struct { u8 unk0[0x10]; u32 flags; u32 index; s32 unk18; u32 unk1C; s32 unk20; s32 unk24; UnkTable *table; } UnkStruct_02040e80;
static inline s32 GetEntryVal(UnkTable *t, u32 i) { if (i >= t->count) return 0; { UnkEntry *e = (UnkEntry *)((u8 *)t + t->offset); return e[i].unk4; } }
void KeyframeAnim_PlayReverse(UnkStruct_02040e80 *s) { s32 n = GetEntryVal(s->table, s->index); if (n > 0) n--; else n = 0; s->flags &= ~0x41; s->flags |= 0x28; s->unk18 = n << 12; s->unk20 = -1; s->unk24 = -1; }

#pragma thumb reset
