#include <nitro/types.h>

typedef struct { u32 a; u32 b; } UnkFileId_0201d868;
typedef struct { u8 unk0[0x30]; u32 unk30; u8 file[0x44]; UnkFileId_0201d868 fileId; u32 unk80; u32 unk84; u32 unk88; } UnkStruct_0201d868;
extern UnkStruct_0201d868 *data_02083f04;
BOOL func_0200bc44(UnkFileId_0201d868 *id, const char *path);
void func_0200be70(void *f);
BOOL func_0200bb70(void *f, UnkFileId_0201d868 id);
BOOL func_0201d64c(UnkStruct_0201d868 *s, u32 a, u32 b);

void func_0201d868(UnkStruct_0201d868 *s, const char *path, u32 a, u32 b) {
    s->unk88 = 0;
    s->unk80 = 0;
    s->unk84 = 0;
    if (!func_0200bc44(&s->fileId, path)) return;
    func_0200be70(s->file);
    if (!func_0200bb70(s->file, s->fileId)) return;
    s->unk30 = 1;
    if (func_0201d64c(s, a, b)) {
        data_02083f04 = s;
    }
}
