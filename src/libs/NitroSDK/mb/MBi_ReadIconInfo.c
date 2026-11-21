#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;
    u32 bottom;
    u8 pad28[0x20];
} UnkFSFile_02018128;

void FS_InitFile(UnkFSFile_02018128 *f);
BOOL FS_OpenFile(UnkFSFile_02018128 *f, const char *path);
s32 FS_ReadFile(UnkFSFile_02018128 *f, void *dst, u32 size);
void FS_CloseFile(UnkFSFile_02018128 *f);

BOOL MBi_ReadIconInfo(const char *path, u8 *dst, BOOL big)
{
    UnkFSFile_02018128 file;
    u32 size = big ? 0x200 : 0x20;
    if (big) {
        dst += 0x20;
    }
    if (path == NULL) {
        return FALSE;
    }
    FS_InitFile(&file);
    if (!FS_OpenFile(&file, path)) {
        return FALSE;
    }
    if (size != file.bottom - file.top) {
        FS_CloseFile(&file);
        return FALSE;
    }
    FS_ReadFile(&file, dst, size);
    FS_CloseFile(&file);
    return TRUE;
}
