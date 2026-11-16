#include <nitro/types.h>

typedef struct UnkFSFile_0200b2f4 {
    u8 pad0[0x14];
    s32 error;   /* 0x14 */
} UnkFSFile_0200b2f4;

typedef struct {
    u8 pad0[0xc];
    u16 syncQ;   /* 0x0c */
    u8 padE[2];
    vu32 flag;   /* 0x10 */
    UnkFSFile_0200b2f4 *listPrev; /* 0x14 */
    UnkFSFile_0200b2f4 *listNext; /* 0x18 */
} UnkFSArchive_0200b2f4;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void OS_WakeupThread(void *queue);
void FSi_ReleaseCommand(UnkFSFile_0200b2f4 *f, s32 ret);
UnkFSFile_0200b2f4 *FSi_NextCommand(UnkFSArchive_0200b2f4 *p_arc);
void FSi_ExecuteAsyncCommand(UnkFSFile_0200b2f4 *f);

static inline BOOL func_0200b2f4_IsAsync(UnkFSArchive_0200b2f4 *a)
{
    return (a->flag & 0x100) ? TRUE : FALSE;
}

void FS_NotifyArchiveAsyncEnd(UnkFSArchive_0200b2f4 *p_arc, s32 ret)
{
    if (func_0200b2f4_IsAsync(p_arc) != FALSE) {
        UnkFSFile_0200b2f4 *const p_file = p_arc->listNext;
        p_arc->flag &= ~0x100;
        FSi_ReleaseCommand(p_file, ret);
        {
            UnkFSFile_0200b2f4 *const p_next = FSi_NextCommand(p_arc);
            if (p_next) {
                FSi_ExecuteAsyncCommand(p_next);
            }
        }
    } else {
        UnkFSFile_0200b2f4 *const p_file = p_arc->listNext;
        u32 bak_psr = OS_DisableInterrupts();
        p_file->error = ret;
        p_arc->flag &= ~0x200;
        OS_WakeupThread(&p_arc->syncQ);
        (void)OS_RestoreInterrupts(bak_psr);
    }
}
