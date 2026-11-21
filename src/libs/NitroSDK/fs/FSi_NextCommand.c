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
void OS_WakeupThread(void *queue);

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

void FSi_ReleaseCommand(UnkFSFile *f, s32 ret);
void FS_InitFile(UnkFSFile *f);

UnkFSFile *FSi_NextCommand(UnkFSArchive *p_arc)
{
    u32 bak_psr = OS_DisableInterrupts();

    if (IsArcCanceling(p_arc) != FALSE) {
        UnkFSFile *p;
        UnkFSFile *q;
        p_arc->flag &= ~0x20;
        for (p = p_arc->listNext; p; p = q) {
            q = p->next;
            if (IsCanceling(p) != FALSE) {
                if (p_arc->listNext == p) {
                    p_arc->listNext = q;
                }
                FSi_ReleaseCommand(p, 3);
                if (!q) {
                    q = p_arc->listNext;
                }
            }
        }
    }
    if (IsArcSuspending(p_arc) == FALSE && IsArcSuspended(p_arc) == FALSE) {
        UnkFSFile *p_file = p_arc->listNext;
        if (p_file != NULL) {
            const BOOL is_start = !IsArcRunning(p_arc);
            if (is_start) {
                p_arc->flag |= 0x10;
            }
            (void)OS_RestoreInterrupts(bak_psr);
            if (is_start) {
                if ((p_arc->procFlag & 0x200) != 0) {
                    (void)(*p_arc->proc)(p_file, 9);
                }
            }
            bak_psr = OS_DisableInterrupts();
            p_file->stat |= 0x40;
            if (IsSync(p_file) != FALSE) {
                OS_WakeupThread(&p_file->queue);
                (void)OS_RestoreInterrupts(bak_psr);
                return NULL;
            }
            (void)OS_RestoreInterrupts(bak_psr);
            return p_file;
        }
    }
    if (IsArcRunning(p_arc) != FALSE) {
        p_arc->flag &= ~0x10;
        if ((p_arc->procFlag & 0x400) != 0) {
            UnkFSFile tmp;
            FS_InitFile(&tmp);
            tmp.arc = p_arc;
            (void)(*p_arc->proc)(&tmp, 10);
        }
    }
    if (IsArcSuspending(p_arc) != FALSE) {
        p_arc->flag &= ~0x40;
        p_arc->flag |= 8;
        OS_WakeupThread(&p_arc->statQ);
    }
    (void)OS_RestoreInterrupts(bak_psr);
    return NULL;
}
