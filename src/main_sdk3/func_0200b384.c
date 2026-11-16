#include <nitro/types.h>

typedef struct UnkFSFile_0200b384 UnkFSFile_0200b384;

typedef struct {
    u8 pad0[0x10];
    vu32 flag;         /* 0x10 */
    u8 list[0x34];     /* 0x14 */
    s32 (*proc)(UnkFSFile_0200b384 *f, u32 command); /* 0x48 */
    u32 procFlag;      /* 0x4c */
} UnkFSArchive_0200b384;

struct UnkFSFile_0200b384 {
    u8 pad0[8];
    UnkFSArchive_0200b384 *arc; /* 0x08 */
    vu32 stat;                  /* 0x0c */
    u32 command;                /* 0x10 */
    s32 error;                  /* 0x14 */
    u32 queue;                  /* 0x18 */
};

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_02005c48(void *queue);
void func_0200aea4(UnkFSFile_0200b384 *f, s32 ret);
void func_0200bed4(UnkFSFile_0200b384 *f, void *list);
void func_0200b55c(UnkFSFile_0200b384 *f);
BOOL func_0200b514(UnkFSFile_0200b384 *f);

static inline BOOL func_0200b384_IsSync(UnkFSFile_0200b384 *f)
{
    return (f->stat & 4) ? TRUE : FALSE;
}

static inline BOOL func_0200b384_IsSuspended(UnkFSArchive_0200b384 *a)
{
    return (a->flag & 8) ? TRUE : FALSE;
}

static inline BOOL func_0200b384_IsRunning(UnkFSArchive_0200b384 *a)
{
    return (a->flag & 0x10) ? TRUE : FALSE;
}

BOOL func_0200b384(UnkFSFile_0200b384 *p_file, u32 command)
{
    UnkFSArchive_0200b384 *const p_arc = p_file->arc;
    const s32 bit = (1 << command);

    p_file->command = command;
    p_file->error = 2;
    p_file->stat |= 1;
    {
        u32 bak_psr = OS_DisableInterrupts();
        if (p_arc->flag & 0x80) {
            func_0200aea4(p_file, 3);
            (void)OS_RestoreInterrupts(bak_psr);
            return FALSE;
        }
        if ((bit & 0x1fc) != 0) {
            p_file->stat |= 4;
        }
        func_0200bed4(p_file, p_arc->list);
        if (func_0200b384_IsSuspended(p_arc) == FALSE && func_0200b384_IsRunning(p_arc) == FALSE) {
            p_arc->flag |= 0x10;
            (void)OS_RestoreInterrupts(bak_psr);
            if ((p_arc->procFlag & 0x200) != 0) {
                (void)(*p_arc->proc)(p_file, 9);
            }
            bak_psr = OS_DisableInterrupts();
            p_file->stat |= 0x40;
            if (func_0200b384_IsSync(p_file) == FALSE) {
                (void)OS_RestoreInterrupts(bak_psr);
                func_0200b55c(p_file);
                return TRUE;
            }
            (void)OS_RestoreInterrupts(bak_psr);
        } else if (func_0200b384_IsSync(p_file) == FALSE) {
            (void)OS_RestoreInterrupts(bak_psr);
            return TRUE;
        } else {
            do {
                func_02005c48(&p_file->queue);
            } while (!(p_file->stat & 0x40));
            (void)OS_RestoreInterrupts(bak_psr);
        }
    }
    return func_0200b514(p_file);
}
