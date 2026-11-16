#include <nitro/types.h>

typedef struct UnkFSFile_0200b034 {
    struct UnkFSFile_0200b034 *prev;
    struct UnkFSFile_0200b034 *next;
} UnkFSFile_0200b034;

typedef struct {
    u8 pad0[0x10];
    vu32 flag;                   /* 0x10 */
    UnkFSFile_0200b034 *listPrev; /* 0x14 */
    UnkFSFile_0200b034 *listNext; /* 0x18 */
    u32 base;                    /* 0x1c */
    u32 fat;                     /* 0x20 */
    u32 fatSize;                 /* 0x24 */
    u32 fnt;                     /* 0x28 */
    u32 fntSize;                 /* 0x2c */
    u32 fatBak;                  /* 0x30 */
    u32 fntBak;                  /* 0x34 */
} UnkFSArchive_0200b034;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
BOOL FS_SuspendArchive(UnkFSArchive_0200b034 *a);
void FS_ResumeArchive(UnkFSArchive_0200b034 *a);
void FSi_ReleaseCommand(UnkFSFile_0200b034 *f, s32 ret);

static inline BOOL func_0200b034_IsLoaded(UnkFSArchive_0200b034 *a)
{
    return (a->flag & 2) ? TRUE : FALSE;
}

BOOL FS_UnloadArchive(UnkFSArchive_0200b034 *p_arc)
{
    u32 bak_psr = OS_DisableInterrupts();

    if (func_0200b034_IsLoaded(p_arc) != FALSE) {
        {
            UnkFSFile_0200b034 *p;
            UnkFSFile_0200b034 *q;
            BOOL bak_state;
            (void)p_arc->flag;
            bak_state = FS_SuspendArchive(p_arc);
            p_arc->flag |= 0x80;
            for (p = p_arc->listNext; p; p = q) {
                q = p->next;
                FSi_ReleaseCommand(p, 3);
            }
            p_arc->listNext = NULL;
            if (bak_state) {
                FS_ResumeArchive(p_arc);
            }
        }
        p_arc->base = 0;
        p_arc->fat = 0;
        p_arc->fatSize = 0;
        p_arc->fnt = 0;
        p_arc->fntSize = 0;
        p_arc->fatBak = p_arc->fntBak = 0;
        p_arc->flag &= ~0xa2;
    }
    (void)OS_RestoreInterrupts(bak_psr);
    return TRUE;
}
