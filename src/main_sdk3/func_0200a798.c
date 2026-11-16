#include <nitro/types.h>

typedef struct {
    u32 arc;
    u16 ownId;
    u16 index;
    u32 pos;
} UnkFSDirPos_0200a798;

typedef struct {
    u32 arc;
    u32 fileId;
} UnkFSFileID_0200a798;

typedef struct {
    union {
        UnkFSFileID_0200a798 fileId;
        UnkFSDirPos_0200a798 dirId;
    } id;           /* 0x00 */
    u32 isDir;      /* 0x0c */
    u32 nameLen;    /* 0x10 */
    char name[128]; /* 0x14 */
} UnkFSDirEntry_0200a798;

typedef struct {
    u8 pad0[0x1c];
    UnkFSDirPos_0200a798 pos; /* 0x1c */
    u32 parent;               /* 0x28 */
    union {
        struct {
            UnkFSDirEntry_0200a798 *entry;
            BOOL skipString;
        } readdir;
        struct {
            UnkFSDirPos_0200a798 pos;
        } seekdir;
        struct {
            u8 pad[0xc];
            const u8 *path;   /* 0x38 */
            BOOL findDir;       /* 0x3c */
            void *result;       /* 0x40 */
        } findpath;
    } arg;                    /* 0x2c */
} UnkFSFile_0200a798;

void func_0200ac30(UnkFSFile_0200a798 *f, u32 id);
s32 func_0200ad14(UnkFSFile_0200a798 *f, u32 cmd);
s32 func_0200abcc(const u8 *a, const char *b, u32 len);

static inline BOOL func_0200a798_IsChar(u32 c)
{
    BOOL r = FALSE;
    if (c != 0 && c != '/' && c != '\\') {
        r = TRUE;
    }
    return r;
}

s32 func_0200a798(UnkFSFile_0200a798 *file)
{
    const u8 *path = file->arg.findpath.path;
    BOOL findDir = file->arg.findpath.findDir;
    UnkFSDirEntry_0200a798 entry;

    func_0200ad14(file, 2);
    while (*path != 0) {
        BOOL isDir;
        u32 c;
        s32 len;
        for (len = 0; func_0200a798_IsChar(c = path[len]); len++) {
        }
        if (c != 0 || findDir) {
            c = TRUE;
        }
        isDir = c;
        if (len == 0) {
            return 1;
        }
        if (path[0] == '.') {
            if (len == 1) {
                path += 1;
                goto next;
            }
            if ((len == 2) & (path[1] == '.')) {
                if (file->pos.ownId != 0) {
                    func_0200ac30(file, file->parent);
                }
                path += 2;
                goto next;
            }
        }
        if (len > 127) {
            return 1;
        }
        file->arg.readdir.entry = &entry;
        file->arg.readdir.skipString = FALSE;
        for (;;) {
            if (func_0200ad14(file, 3) != 0) {
                return 1;
            }
            if (isDir == entry.isDir && len == entry.nameLen && func_0200abcc(path, entry.name, len) == 0) {
                break;
            }
        }
        if (isDir) {
            file->arg.seekdir.pos = entry.id.dirId;
            path += len;
            func_0200ad14(file, 2);
        } else {
            if (findDir) {
                return 1;
            }
            *(UnkFSFileID_0200a798 *)file->arg.findpath.result = entry.id.fileId;
            return 0;
        }
    next:
        path += (*path != 0) ? 1 : 0;
    }
    if (!findDir) {
        return 1;
    }
    *(UnkFSDirPos_0200a798 *)file->arg.findpath.result = file->pos;
    return 0;
}
