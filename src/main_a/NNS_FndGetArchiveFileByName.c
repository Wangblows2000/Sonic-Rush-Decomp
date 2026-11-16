#include <nitro/types.h>

typedef struct { u8 unk0[0x58]; u32 unk58; } UnkArc_0202128c;
typedef struct { u32 unk0; u32 unk4; UnkArc_0202128c *unk8; u8 unkC[0x14]; u32 unk20; u8 unk24[0x20]; } UnkFile_0202128c;
void FS_InitFile(UnkFile_0202128c *f);
BOOL FS_OpenFile(UnkFile_0202128c *f, void *path);
void FS_CloseFile(UnkFile_0202128c *f);

u32 NNS_FndGetArchiveFileByName(void *path) {
    UnkFile_0202128c f;
    u32 r = 0;
    FS_InitFile(&f);
    if (FS_OpenFile(&f, path)) {
        r = f.unk8->unk58 + f.unk20;
        FS_CloseFile(&f);
    }
    return r;
}
