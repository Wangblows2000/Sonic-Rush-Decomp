#include <nitro/types.h>

typedef struct UnkFSFile UnkFSFile;

typedef struct {
    u8 pad0[0xe];
    u16 statQ;         /* 0x0e */
    vu32 flag;         /* 0x10 */
    UnkFSFile *listPrev; /* 0x14 */
    UnkFSFile *listNext; /* 0x18 */
    u8 pad1c[0x2c];
    s32 (*proc)(UnkFSFile *f, u32 command); /* 0x48 */
    u32 procFlag;      /* 0x4c */
} UnkFSArchive;

struct UnkFSFile {
    UnkFSFile *prev;   /* 0x00 */
    UnkFSFile *next;   /* 0x04 */
    UnkFSArchive *arc; /* 0x08 */
    vu32 stat;         /* 0x0c */
    u32 command;       /* 0x10 */
    s32 error;         /* 0x14 */
    u16 queue;         /* 0x18 */
    u8 pad1a[0x44 - 0x1a];
};

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_02005bd4(void *queue);

static inline BOOL IsSync(UnkFSFile *f)
{
    return (f->stat & 4) ? TRUE : FALSE;
}
static inline BOOL IsCanceling(UnkFSFile *f)
{
    return (f->stat & 2) ? TRUE : FALSE;
}
static inline BOOL IsArcCanceling(UnkFSArchive *a)
{
    return (a->flag & 0x20) ? TRUE : FALSE;
}
static inline BOOL IsArcSuspending(UnkFSArchive *a)
{
    return (a->flag & 0x40) ? TRUE : FALSE;
}
static inline BOOL IsArcSuspended(UnkFSArchive *a)
{
    return (a->flag & 8) ? TRUE : FALSE;
}
static inline BOOL IsArcRunning(UnkFSArchive *a)
{
    return (a->flag & 0x10) ? TRUE : FALSE;
}

s32 func_0200ad14(UnkFSFile *p_file, u32 command);
UnkFSFile *func_0200b600(UnkFSArchive *p_arc);

void func_0200b55c(UnkFSFile *p_file)
{
    UnkFSArchive *const p_arc = p_file->arc;

    while (p_file) {
        u32 bak_psr = OS_DisableInterrupts();
        p_file->stat |= 0x40;
        if (IsSync(p_file) != FALSE) {
            func_02005bd4(&p_file->queue);
            (void)OS_RestoreInterrupts(bak_psr);
            break;
        }
        p_file->stat |= 8;
        (void)OS_RestoreInterrupts(bak_psr);
        if (func_0200ad14(p_file, p_file->command) == 6) {
            break;
        }
        p_file = func_0200b600(p_arc);
    }
}
