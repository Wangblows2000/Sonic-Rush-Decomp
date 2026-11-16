#include <nitro/types.h>

typedef struct {
    void *arc;
    u16 ownId;
    u16 index;
    u32 pos;
} UnkFSDirPos_0200bd10;

typedef struct {
    u8 pad0[0x10];
    vu32 flag;   /* 0x10 */
} UnkFSArchive_0200bd10;

typedef struct {
    u8 pad0[8];
    void *arc;                     /* 0x08 */
    u8 padC[0x20];
    UnkFSDirPos_0200bd10 pos;      /* 0x2c */
    const char *path;              /* 0x38 */
    BOOL findDirectory;            /* 0x3c */
    void *result;                  /* 0x40 */
} UnkFSFile_0200bd10;

extern UnkFSDirPos_0200bd10 data_0207efac;

UnkFSArchive_0200bd10 *func_0200b828(const char *name, s32 len);
BOOL func_0200b384(UnkFSFile_0200bd10 *f, u32 command);

static inline BOOL func_0200bd10_IsLoaded(UnkFSArchive_0200bd10 *a)
{
    return (a->flag & 2) ? TRUE : FALSE;
}

static inline BOOL func_0200bd10_IsSlash(u32 c)
{
    return (c == '/') || (c == '\\');
}

BOOL func_0200bd10(UnkFSFile_0200bd10 *p_dir, const char *path, void *p_file_id, void *p_dir_pos)
{
    UnkFSDirPos_0200bd10 pos;

    if (func_0200bd10_IsSlash((u8)*path)) {
        pos.arc = data_0207efac.arc;
        pos.ownId = 0;
        pos.pos = 0;
        pos.index = 0;
        ++path;
    } else {
        s32 i;
        pos = data_0207efac;
        for (i = 0; i <= 3; ++i) {
            u32 c = (u8)path[i];
            if (!c || func_0200bd10_IsSlash(c)) {
                break;
            } else if (c == ':') {
                UnkFSArchive_0200bd10 *const p_arc = func_0200b828(path, i);
                if (!p_arc) {
                    return FALSE;
                } else if (func_0200bd10_IsLoaded(p_arc) == FALSE) {
                    return FALSE;
                }
                pos.arc = p_arc;
                pos.pos = 0;
                pos.index = 0;
                pos.ownId = 0;
                path += i + 1;
                if (func_0200bd10_IsSlash((u8)*path)) {
                    ++path;
                }
                break;
            }
        }
    }
    p_dir->arc = pos.arc;
    p_dir->path = path;
    p_dir->pos = pos;
    if (p_dir_pos) {
        p_dir->findDirectory = TRUE;
        p_dir->result = p_dir_pos;
    } else {
        p_dir->findDirectory = FALSE;
        p_dir->result = p_file_id;
    }
    return func_0200b384(p_dir, 4);
}
