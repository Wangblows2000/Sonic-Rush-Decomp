#include <nitro/types.h>

typedef struct UnkFSArchive_0200ac64 {
    u8 pad0[0xc];
    u16 syncQ;   /* 0x0c */
    u8 padE[2];
    vu32 flag;   /* 0x10 */
    u8 pad14[0x30];
    s32 (*tableFunc)(struct UnkFSArchive_0200ac64 *a, void *dst, u32 pos, u32 len); /* 0x44 */
} UnkFSArchive_0200ac64;

typedef struct {
    UnkFSArchive_0200ac64 *arc;
    u32 pos;
} UnkFSReadParam_0200ac64;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void OS_SleepThread(void *queue);

static inline BOOL func_0200ac64_IsSync(UnkFSArchive_0200ac64 *a)
{
    return (a->flag & 0x200) ? TRUE : FALSE;
}

void FSi_ReadTable(UnkFSReadParam_0200ac64 *p, void *dst, u32 len)
{
    s32 result;
    UnkFSArchive_0200ac64 *const p_arc = p->arc;

    p_arc->flag |= 0x200;
    switch (result = (*p_arc->tableFunc)(p_arc, dst, p->pos, len)) {
    case 0:
    case 1:
        p_arc->flag &= ~0x200;
        break;
    case 6: {
        u32 bak_psr = OS_DisableInterrupts();
        while (func_0200ac64_IsSync(p_arc) != FALSE) {
            OS_SleepThread(&p_arc->syncQ);
        }
        (void)OS_RestoreInterrupts(bak_psr);
        break;
    }
    }
    p->pos += len;
}
