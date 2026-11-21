#include <nitro/types.h>

typedef struct UnkFSArchive_0200b174 {
    u32 name;                            /* 0x00 */
    struct UnkFSArchive_0200b174 *next;  /* 0x04 */
    struct UnkFSArchive_0200b174 *prev;  /* 0x08 */
    u8 padC[4];
    vu32 flag;                           /* 0x10 */
} UnkFSArchive_0200b174;

typedef struct {
    void *arc;
    u16 ownId;
    u16 index;
    u32 pos;
} UnkFSDirPos_0200b174;

extern UnkFSArchive_0200b174 *data_0207efa8;
extern UnkFSDirPos_0200b174 data_0207efac;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);

void FS_ReleaseArchiveName(UnkFSArchive_0200b174 *p_arc)
{
    if (p_arc->name) {
        u32 bak_psr = OS_DisableInterrupts();
        if (p_arc->next) {
            p_arc->next->prev = p_arc->prev;
        }
        if (p_arc->prev) {
            p_arc->prev->next = p_arc->next;
        }
        p_arc->name = 0;
        p_arc->next = p_arc->prev = NULL;
        p_arc->flag &= ~1;
        if (data_0207efac.arc == p_arc) {
            data_0207efac.arc = data_0207efa8;
            data_0207efac.pos = 0;
            data_0207efac.index = 0;
            data_0207efac.ownId = 0;
        }
        (void)OS_RestoreInterrupts(bak_psr);
    }
}
