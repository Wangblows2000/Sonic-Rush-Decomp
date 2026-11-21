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

void OS_SleepThread(void *queue);

BOOL FS_SuspendArchive(UnkFSArchive *p_arc)
{
    u32 bak_psr = OS_DisableInterrupts();
    const BOOL bak_stat = !IsArcSuspended(p_arc);

    if (bak_stat) {
        if (IsArcRunning(p_arc) != FALSE) {
            p_arc->flag |= 0x40;
            do {
                OS_SleepThread(&p_arc->statQ);
            } while (IsArcSuspending(p_arc) != FALSE);
        } else {
            p_arc->flag |= 8;
        }
    }
    (void)OS_RestoreInterrupts(bak_psr);
    return bak_stat;
}
