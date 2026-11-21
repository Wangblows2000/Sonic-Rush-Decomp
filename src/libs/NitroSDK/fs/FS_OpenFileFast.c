#include <nitro/types.h>

typedef struct {
    void *arc;
    u32 fileId;
} UnkFSFileID_0200bb70;

typedef struct {
    u8 pad0[8];
    void *arc;                 /* 0x08 */
    vu32 stat;                 /* 0x0c */
    u8 pad10[0x1c];
    UnkFSFileID_0200bb70 id;   /* 0x2c */
} UnkFSFile_0200bb70;

BOOL FSi_SendCommand(UnkFSFile_0200bb70 *f, u32 command);

BOOL FS_OpenFileFast(UnkFSFile_0200bb70 *p_file, UnkFSFileID_0200bb70 id)
{
    if (!id.arc) {
        return FALSE;
    }
    p_file->arc = id.arc;
    p_file->id = id;
    if (!FSi_SendCommand(p_file, 6)) {
        return FALSE;
    }
    p_file->stat |= 0x10;
    p_file->stat &= ~0x20;
    return TRUE;
}
