#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; u32 top; u32 bottom; u8 unk28[0x20]; } UnkFile_02033ddc;
void FS_InitFile(UnkFile_02033ddc *f);
BOOL FS_OpenFile(UnkFile_02033ddc *f, void *path);
BOOL FS_CloseFile(UnkFile_02033ddc *f);

s32 File_GetSize(void *path) {
    UnkFile_02033ddc f;
    s32 len;
    FS_InitFile(&f);
    if (!FS_OpenFile(&f, path)) return -1;
    len = f.bottom - f.top;
    if (!FS_CloseFile(&f)) len = -1;
    return len;
}
