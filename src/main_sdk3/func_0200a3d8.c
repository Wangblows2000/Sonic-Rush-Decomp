#include <nitro/types.h>

typedef struct {
    u32 arc;        /* 0x00 */
    union {
        u32 fileId; /* 0x04 */
        u16 ownId;
    } id;
    u32 unk8;       /* 0x08 */
    u32 isDir;      /* 0x0c */
    u32 nameLen;    /* 0x10 */
    char name[128]; /* 0x14 */
} UnkFSDirEntry_0200a3d8;

typedef struct {
    u8 *buf;
    u32 bufLen;
    u16 totalLen;
    u16 dirId;
} UnkGetPath_0200a3d8;

typedef struct {
    u8 pad0[8];
    u32 *arc;       /* 0x08 */
    u32 stat;       /* 0x0c */
    u8 pad10[0xc];
    u32 fileId;     /* 0x1c */
    u16 ownId;      /* 0x20 */
    u8 pad22[6];
    u32 parent;     /* 0x28 */
    union {
        UnkGetPath_0200a3d8 getpath;
        struct {
            UnkFSDirEntry_0200a3d8 *entry;
            BOOL skipString;
        } readdir;
    } arg;          /* 0x2c */
    u8 pad38[0x44 - 0x38];
} UnkFSFile_0200a3d8;

extern char data_0207797c[];

void func_0200be70(UnkFSFile_0200a3d8 *f);
void func_0200ac30(UnkFSFile_0200a3d8 *f, u32 id);
s32 func_0200ad14(UnkFSFile_0200a3d8 *f, u32 cmd);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

static inline BOOL func_0200a3d8_IsFile(UnkFSFile_0200a3d8 *f)
{
    return (f->stat & 0x20) ? TRUE : FALSE;
}

s32 func_0200a3d8(UnkFSFile_0200a3d8 *file)
{
    u32 *arc = file->arc;
    UnkGetPath_0200a3d8 *info = &file->arg.getpath;
    UnkFSDirEntry_0200a3d8 entry;
    UnkFSFile_0200a3d8 tmp;
    u32 dirId;
    u32 fileId;
    u8 *buf;
    s32 len;

    func_0200be70(&tmp);
    tmp.arc = file->arc;
    if (func_0200a3d8_IsFile(file) != FALSE) {
        dirId = file->ownId;
        fileId = 0x10000;
    } else {
        fileId = file->fileId;
        if (info->totalLen != 0) {
            dirId = info->dirId;
        } else {
            u32 i = 0;
            u32 n = 0;
            dirId = 0x10000;
            do {
                func_0200ac30(&tmp, i);
                if (i == 0) {
                    n = tmp.parent;
                }
                tmp.arg.readdir.entry = &entry;
                tmp.arg.readdir.skipString = TRUE;
                while (func_0200ad14(&tmp, 3) == 0) {
                    if (!entry.isDir && entry.id.fileId == fileId) {
                        dirId = tmp.ownId;
                        break;
                    }
                }
                if (dirId != 0x10000) {
                    break;
                }
            } while (++i < n);
        }
    }
    if (dirId == 0x10000) {
        info->totalLen = 0;
        return 1;
    }
    if (info->totalLen == 0) {
        u32 id;
        u32 total = 0;
        if (*arc <= 0xff) {
            total += 1;
        } else if (*arc <= 0xff00) {
            total += 2;
        } else {
            total += 3;
        }
        total += 2;
        if (fileId != 0x10000) {
            total += entry.nameLen;
        }
        id = dirId;
        if (id != 0) {
            func_0200ac30(&tmp, id);
            do {
                func_0200ac30(&tmp, tmp.parent);
                tmp.arg.readdir.entry = &entry;
                tmp.arg.readdir.skipString = TRUE;
                while (func_0200ad14(&tmp, 3) == 0) {
                    if (entry.isDir && entry.id.ownId == id) {
                        total += entry.nameLen + 1;
                        break;
                    }
                }
                id = tmp.ownId;
            } while (id != 0);
        }
        info->totalLen = total + 1;
        info->dirId = dirId;
    }
    buf = info->buf;
    if (buf == NULL) {
        return 0;
    }
    len = info->totalLen;
    if (info->bufLen < len) {
        return 1;
    }
    {
        u32 pos = 0;
        u32 arcLen;
        if (*arc <= 0xff) {
            arcLen = 1;
        } else if (*arc <= 0xff00) {
            arcLen = 2;
        } else {
            arcLen = 3;
        }
        MI_CpuCopy8(arc, buf + pos, arcLen);
        pos += arcLen;
        MI_CpuCopy8(data_0207797c, buf + pos, 2);
    }
    func_0200ac30(&tmp, dirId);
    if (fileId != 0x10000) {
        u32 n;
        tmp.arg.readdir.entry = &entry;
        tmp.arg.readdir.skipString = FALSE;
        while (func_0200ad14(&tmp, 3) == 0) {
            if (!entry.isDir && entry.id.fileId == fileId) {
                break;
            }
        }
        n = entry.nameLen + 1;
        MI_CpuCopy8(entry.name, buf + len - n, n);
        len -= n;
    } else {
        buf[len - 1] = 0;
        len -= 1;
    }
    if (dirId != 0) {
        do {
            func_0200ac30(&tmp, tmp.parent);
            tmp.arg.readdir.entry = &entry;
            tmp.arg.readdir.skipString = FALSE;
            buf[len - 1] = '/';
            len -= 1;
            while (func_0200ad14(&tmp, 3) == 0) {
                if (entry.isDir && entry.id.ownId == dirId) {
                    u32 n = entry.nameLen;
                    MI_CpuCopy8(entry.name, buf + len - n, n);
                    len -= n;
                    break;
                }
            }
            dirId = tmp.ownId;
        } while (dirId != 0);
    }
    return 0;
}
