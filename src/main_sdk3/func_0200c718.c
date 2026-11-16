#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;       /* 0x20 */
    u32 bottom;    /* 0x24 */
    u8 pad28[0x48 - 0x28];
} UnkFSFile_0200c718;

typedef struct {
    u32 arc;
    u32 fileId;
} UnkFSFileID_0200c718;

typedef struct {
    u8 header[0x20];
    u32 target;    /* 0x20 */
    u32 filePos;   /* 0x24 */
    u32 fileSize;  /* 0x28 */
} UnkFSOverlayInfo_0200c718;

void func_0200be70(UnkFSFile_0200c718 *f);
BOOL func_0200bbec(UnkFSFile_0200c718 *f, void *arc, u32 top, u32 bottom, s32 id);
s32 func_0200b99c(UnkFSFile_0200c718 *f, void *dst, u32 size);
void func_0200bad8(UnkFSFile_0200c718 *f);
void FS_GetOverlayFileID(UnkFSFileID_0200c718 *id, UnkFSOverlayInfo_0200c718 *ovi);
BOOL func_0200bb70(UnkFSFile_0200c718 *f, UnkFSFileID_0200c718 id);

BOOL func_0200c718(UnkFSOverlayInfo_0200c718 *p_ovi, u32 target, u32 id, void *p_arc, u32 offset_arm9, u32 len_arm9, u32 offset_arm7, u32 len_arm7)
{
    u32 pos;
    u32 offset;
    u32 len;
    UnkFSFile_0200c718 file;

    if (target == 0) {
        offset = offset_arm9;
        len = len_arm9;
    } else {
        offset = offset_arm7;
        len = len_arm7;
    }
    pos = id * 0x20;
    if (pos >= len) {
        return FALSE;
    }
    func_0200be70(&file);
    if (!func_0200bbec(&file, p_arc, offset + pos, offset + len, -1)) {
        return FALSE;
    }
    if (func_0200b99c(&file, p_ovi, 0x20) != 0x20) {
        func_0200bad8(&file);
        return FALSE;
    }
    func_0200bad8(&file);
    p_ovi->target = target;
    {
        UnkFSFileID_0200c718 file_id;
        FS_GetOverlayFileID(&file_id, p_ovi);
        if (!func_0200bb70(&file, file_id)) {
            return FALSE;
        }
    }
    p_ovi->filePos = file.top;
    p_ovi->fileSize = file.bottom - file.top;
    func_0200bad8(&file);
    return TRUE;
}
