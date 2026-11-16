#include <nitro/types.h>

typedef struct { u32 offset; u32 size; u32 unk8; u32 unkC; } UnkEntry_0201d140;
typedef struct { u32 unk0; u32 unk4; u32 count; UnkEntry_0201d140 entries[1]; } UnkTable_0201d140;
typedef struct { u8 unk0[0x34]; u8 file[0x4C]; UnkTable_0201d140 *unk80; } UnkStruct_0201d140;
extern UnkStruct_0201d140 *data_02083f04;
BOOL FS_SeekFile(void *f, u32 pos, u32 whence);
s32 FS_ReadFile(void *f, void *buf, u32 size);

s32 NNS_SndArcReadFile(u32 idx, void *buf, u32 size, u32 pos) {
    UnkStruct_0201d140 *g = data_02083f04;
    UnkTable_0201d140 *t = g->unk80;
    UnkEntry_0201d140 *e;
    if (idx >= t->count) return -1;
    e = &t->entries[idx];
    if (size > e->size - pos) size = e->size - pos;
    if (!FS_SeekFile(g->file, e->offset + pos, 0)) return -1;
    return FS_ReadFile(g->file, buf, size);
}
