#include <nitro/types.h>

typedef struct UnkFSFile UnkFSFile;

typedef struct {
    u8 pad0[0xe];
    u16 statQ;   /* 0x0e */
    vu32 flag;   /* 0x10 */
} UnkFSArchive;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);

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

UnkFSFile *FSi_NextCommand(UnkFSArchive *p_arc);
void FSi_ExecuteAsyncCommand(UnkFSFile *f);

BOOL FS_ResumeArchive(UnkFSArchive *p_arc)
{
    UnkFSFile *p_target = NULL;
    BOOL bak_stat;
    u32 bak_irq = OS_DisableInterrupts();

    bak_stat = !IsArcSuspended(p_arc);
    if (!bak_stat) {
        p_arc->flag &= ~8;
        p_target = FSi_NextCommand(p_arc);
    }
    (void)OS_RestoreInterrupts(bak_irq);
    if (p_target) {
        FSi_ExecuteAsyncCommand(p_target);
    }
    return bak_stat;
}
