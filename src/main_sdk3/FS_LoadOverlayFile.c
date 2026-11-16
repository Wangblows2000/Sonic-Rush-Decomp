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

void func_0200be70(UnkFSFile_0200c564 *f);
void func_0200bad8(UnkFSFile_0200c564 *f);
s32 func_0200b99c(UnkFSFile_0200c564 *f, void *dst, u32 size);
void FS_GetOverlayFileID(UnkFSFileID_0200c564 *id, UnkFSOverlayInfo_0200c564 *ovi);
BOOL func_0200bb70(UnkFSFile_0200c564 *f, UnkFSFileID_0200c564 id);
u32 FSi_GetOverlayBinarySize(UnkFSOverlayInfo_0200c564 *ovi);
void func_0200c850(UnkFSOverlayInfo_0200c564 *ovi);

BOOL FS_LoadOverlayFile(UnkFSOverlayInfo_0200c564 *p_ovi)
{
    UnkFSFileID_0200c564 file_id;
    UnkFSFile_0200c564 file;
    u32 size;
    func_0200be70(&file);
    FS_GetOverlayFileID(&file_id, p_ovi);
    if (!func_0200bb70(&file, file_id)) {
        return FALSE;
    }
    size = FSi_GetOverlayBinarySize(p_ovi);
    func_0200c850(p_ovi);
    if (func_0200b99c(&file, p_ovi->ram_address, size) != size) {
        func_0200bad8(&file);
        return FALSE;
    }
    func_0200bad8(&file);
    return TRUE;
}
