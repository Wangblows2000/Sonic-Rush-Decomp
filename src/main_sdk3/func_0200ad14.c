#include <nitro/types.h>

typedef struct UnkFSFile_0200ad14 UnkFSFile_0200ad14;

typedef struct {
    u8 pad0[0xc];
    u32 syncQ;         /* 0x0c */
    u32 flag;          /* 0x10 */
    u8 pad14[0x34];
    s32 (*proc)(UnkFSFile_0200ad14 *f, u32 command); /* 0x48 */
    u32 procFlag;      /* 0x4c */
} UnkFSArchive_0200ad14;

struct UnkFSFile_0200ad14 {
    u8 pad0[8];
    UnkFSArchive_0200ad14 *arc; /* 0x08 */
    u32 stat;                   /* 0x0c */
    u8 pad10[4];
    s32 error;                  /* 0x14 */
};

extern s32 (*data_020729b4[])(UnkFSFile_0200ad14 *f);

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_02005c48(void *queue);
void func_0200aea4(UnkFSFile_0200ad14 *f, s32 ret);

static inline BOOL func_0200ad14_IsSync(UnkFSFile_0200ad14 *f)
{
    return (f->stat & 4) ? TRUE : FALSE;
}

static inline BOOL func_0200ad14_IsArchiveSync(UnkFSArchive_0200ad14 *a)
{
    return (a->flag & 0x200) ? TRUE : FALSE;
}

s32 func_0200ad14(UnkFSFile_0200ad14 *p_file, u32 command)
{
    s32 ret;
    UnkFSArchive_0200ad14 *const p_arc = p_file->arc;
    const s32 bit = (1 << command);

    if (func_0200ad14_IsSync(p_file) != FALSE) {
        p_arc->flag |= 0x200;
    } else {
        p_arc->flag |= 0x100;
    }
    if ((p_arc->procFlag & bit) != 0) {
        switch (ret = (*p_arc->proc)(p_file, command)) {
        case 0:
        case 1:
        case 4:
            p_file->error = ret;
            break;
        case 8:
            p_arc->procFlag &= ~bit;
            ret = 7;
            break;
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        default:
            break;
        }
    } else {
        ret = 7;
    }
    if (ret == 7) {
        ret = (*data_020729b4[command])(p_file);
    }
    if (ret == 6) {
        if (func_0200ad14_IsSync(p_file) != FALSE) {
            u32 bak = OS_DisableInterrupts();
            while (func_0200ad14_IsArchiveSync(p_arc) != FALSE) {
                func_02005c48(&p_arc->syncQ);
            }
            ret = p_file->error;
            (void)OS_RestoreInterrupts(bak);
        }
    } else if (func_0200ad14_IsSync(p_file) == FALSE) {
        p_arc->flag &= ~0x100;
        func_0200aea4(p_file, ret);
    } else {
        p_arc->flag &= ~0x200;
        p_file->error = ret;
    }
    return ret;
}
