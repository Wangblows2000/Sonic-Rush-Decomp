#include <nitro/types.h>

typedef struct { u32 a; u32 b; } UnkFileId_0201d868;
typedef struct { u8 unk0[0x30]; u32 unk30; u8 file[0x44]; UnkFileId_0201d868 fileId; u32 unk80; u32 unk84; u32 unk88; } UnkStruct_0201d868;
extern UnkStruct_0201d868 *data_02083f04;
BOOL FS_ConvertPathToFileID(UnkFileId_0201d868 *id, const char *path);
void FS_InitFile(void *f);
BOOL FS_OpenFileFast(void *f, UnkFileId_0201d868 id);
BOOL NNS_SndArcSetup(UnkStruct_0201d868 *s, u32 a, u32 b);

void NNS_SndArcInit(UnkStruct_0201d868 *s, const char *path, u32 a, u32 b) {
    s->unk88 = 0;
    s->unk80 = 0;
    s->unk84 = 0;
    if (!FS_ConvertPathToFileID(&s->fileId, path)) return;
    FS_InitFile(s->file);
    if (!FS_OpenFileFast(s->file, s->fileId)) return;
    s->unk30 = 1;
    if (NNS_SndArcSetup(s, a, b)) {
        data_02083f04 = s;
    }
}
