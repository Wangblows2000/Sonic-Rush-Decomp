#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;       /* 0x20 */
    u32 bottom;    /* 0x24 */
    u8 pad28[0x44 - 0x28];
} UnkFSFile_0200c604;

typedef struct {
    u32 arc;
    u32 fileId;
} UnkFSFileID_0200c604;

typedef struct {
    u8 header[0x20];
    u32 target;    /* 0x20 */
    u32 filePos;   /* 0x24 */
    u32 fileSize;  /* 0x28 */
} UnkFSOverlayInfo_0200c604;

typedef struct {
    u8 *ptr;
    u32 len;
} UnkOvtTable_0200c604;

typedef struct {
    u32 offset;
    u32 length;
} UnkRomRegion_0200c604;

extern UnkOvtTable_0200c604 data_0207efc4;
extern UnkOvtTable_0200c604 data_0207efcc;
extern u8 data_0207efd4[];

void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_0200be70(UnkFSFile_0200c604 *f);
void func_0200bad8(UnkFSFile_0200c604 *f);
void FS_GetOverlayFileID(UnkFSFileID_0200c604 *id, UnkFSOverlayInfo_0200c604 *ovi);
BOOL func_0200bb70(UnkFSFile_0200c604 *f, UnkFSFileID_0200c604 id);
BOOL func_0200c718(UnkFSOverlayInfo_0200c604 *p_ovi, u32 target, u32 id, void *p_arc, u32 o9, u32 l9, u32 o7, u32 l7);

BOOL FS_LoadOverlayInfo(UnkFSOverlayInfo_0200c604 *p_ovi, u32 target, u32 id)
{
    UnkOvtTable_0200c604 *table = (target == 0) ? &data_0207efc4 : &data_0207efcc;
    if (table->ptr != NULL) {
        u32 pos = id * 0x20;
        UnkFSFile_0200c604 file;
        UnkFSFileID_0200c604 file_id;
        if (pos >= table->len) {
            return FALSE;
        }
        MI_CpuCopy8(table->ptr + pos, p_ovi, 0x20);
        p_ovi->target = target;
        func_0200be70(&file);
        FS_GetOverlayFileID(&file_id, p_ovi);
        if (!func_0200bb70(&file, file_id)) {
            return FALSE;
        }
        p_ovi->filePos = file.top;
        p_ovi->fileSize = file.bottom - file.top;
        func_0200bad8(&file);
        return TRUE;
    } else {
        const UnkRomRegion_0200c604 *r9 = (const UnkRomRegion_0200c604 *)0x027ffe50;
        const UnkRomRegion_0200c604 *r7 = (const UnkRomRegion_0200c604 *)0x027ffe58;
        return func_0200c718(p_ovi, target, id, data_0207efd4, r9->offset, r9->length, r7->offset, r7->length);
    }
}
