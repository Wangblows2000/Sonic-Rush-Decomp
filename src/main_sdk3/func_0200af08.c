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

UnkFSFile *func_0200b600(UnkFSArchive *p_arc);
void func_0200b55c(UnkFSFile *f);

BOOL func_0200af08(UnkFSArchive *p_arc)
{
    UnkFSFile *p_target = NULL;
    BOOL bak_stat;
    u32 bak_irq = OS_DisableInterrupts();

    bak_stat = !IsArcSuspended(p_arc);
    if (!bak_stat) {
        p_arc->flag &= ~8;
        p_target = func_0200b600(p_arc);
    }
    (void)OS_RestoreInterrupts(bak_irq);
    if (p_target) {
        func_0200b55c(p_target);
    }
    return bak_stat;
}
