#include <nitro/types.h>

typedef struct {
    u8 pad0[0x44];
} UnkFSFile_0200c564;

typedef struct {
    u32 arc;
    u32 fileId;
} UnkFSFileID_0200c564;

typedef struct {
    u32 id;
    void *ram_address;   /* 0x04 */
} UnkFSOverlayInfo_0200c564;

void FS_InitFile(UnkFSFile_0200c564 *f);
void FS_CloseFile(UnkFSFile_0200c564 *f);
s32 FS_ReadFile(UnkFSFile_0200c564 *f, void *dst, u32 size);
void FS_GetOverlayFileID(UnkFSFileID_0200c564 *id, UnkFSOverlayInfo_0200c564 *ovi);
BOOL FS_OpenFileFast(UnkFSFile_0200c564 *f, UnkFSFileID_0200c564 id);
u32 FSi_GetOverlayBinarySize(UnkFSOverlayInfo_0200c564 *ovi);
void FS_ClearOverlayImage(UnkFSOverlayInfo_0200c564 *ovi);

BOOL FS_LoadOverlayFile(UnkFSOverlayInfo_0200c564 *p_ovi)
{
    UnkFSFileID_0200c564 file_id;
    UnkFSFile_0200c564 file;
    u32 size;
    FS_InitFile(&file);
    FS_GetOverlayFileID(&file_id, p_ovi);
    if (!FS_OpenFileFast(&file, file_id)) {
        return FALSE;
    }
    size = FSi_GetOverlayBinarySize(p_ovi);
    FS_ClearOverlayImage(p_ovi);
    if (FS_ReadFile(&file, p_ovi->ram_address, size) != size) {
        FS_CloseFile(&file);
        return FALSE;
    }
    FS_CloseFile(&file);
    return TRUE;
}
