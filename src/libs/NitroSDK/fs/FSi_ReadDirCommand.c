#include <nitro/types.h>

typedef struct {
    u32 arc;
    u16 ownId;
    u16 index;
    u32 pos;
} UnkFSDirPos_0200a9b4;

typedef struct {
    u32 arc;
    u32 fileId;
} UnkFSFileID_0200a9b4;

typedef struct {
    union {
        UnkFSFileID_0200a9b4 fileId;
        UnkFSDirPos_0200a9b4 dirId;
    } id;           /* 0x00 */
    u32 isDir;      /* 0x0c */
    u32 nameLen;    /* 0x10 */
    char name[128]; /* 0x14 */
} UnkFSDirEntry_0200a9b4;

typedef struct {
    u8 pad0[8];
    u32 arc;        /* 0x08 */
    u8 padC[0x16];
    u16 index;      /* 0x22 */
    u32 pos;        /* 0x24 */
    u8 pad28[4];
    UnkFSDirEntry_0200a9b4 *entry; /* 0x2c */
    BOOL skipString; /* 0x30 */
} UnkFSFile_0200a9b4;

typedef struct {
    u32 arc;
    u32 pos;
} UnkFSTbl_0200a9b4;

void FSi_ReadTable(UnkFSTbl_0200a9b4 *tbl, void *dst, u32 len);

s32 FSi_ReadDirCommand(UnkFSFile_0200a9b4 *p_dir)
{
    UnkFSDirEntry_0200a9b4 *p_entry = p_dir->entry;
    UnkFSTbl_0200a9b4 tbl;
    u8 len;

    tbl.arc = p_dir->arc;
    tbl.pos = p_dir->pos;
    FSi_ReadTable(&tbl, &len, 1);
    p_entry->nameLen = len & 0x7f;
    p_entry->isDir = (len >> 7) & 1;
    if (p_entry->nameLen == 0) {
        return 1;
    }
    if (!p_dir->skipString) {
        FSi_ReadTable(&tbl, p_entry->name, p_entry->nameLen);
        p_entry->name[p_entry->nameLen] = '\0';
    } else {
        tbl.pos += p_entry->nameLen;
    }
    if (p_entry->isDir) {
        u16 id;
        FSi_ReadTable(&tbl, &id, 2);
        p_entry->id.dirId.arc = p_dir->arc;
        p_entry->id.dirId.ownId = id & 0x0fff;
        p_entry->id.dirId.index = 0;
        p_entry->id.dirId.pos = 0;
    } else {
        p_entry->id.fileId.arc = p_dir->arc;
        p_entry->id.fileId.fileId = p_dir->index;
        ++p_dir->index;
    }
    p_dir->pos = tbl.pos;
    return 0;
}
