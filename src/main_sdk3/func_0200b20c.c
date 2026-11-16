#include <nitro/types.h>

typedef struct UnkFSArchive_0200b20c {
    u32 name;                            /* 0x00 */
    struct UnkFSArchive_0200b20c *next;  /* 0x04 */
    struct UnkFSArchive_0200b20c *prev;  /* 0x08 */
    u8 padC[4];
    vu32 flag;                           /* 0x10 */
} UnkFSArchive_0200b20c;

typedef struct {
    void *arc;
    u16 ownId;
    u16 index;
    u32 pos;
} UnkFSDirPos_0200b20c;

extern UnkFSArchive_0200b20c *data_0207efa8;
extern UnkFSDirPos_0200b20c data_0207efac;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
UnkFSArchive_0200b20c *func_0200b828(const char *name, s32 len);
u32 func_0200b8cc(const char *name, u32 len);

BOOL func_0200b20c(UnkFSArchive_0200b20c *p_arc, const char *name, u32 name_len)
{
    BOOL ret = FALSE;
    u32 bak_intr = OS_DisableInterrupts();

    if (!func_0200b828(name, (s32)name_len)) {
        if (!data_0207efa8) {
            data_0207efa8 = p_arc;
            data_0207efac.arc = p_arc;
            data_0207efac.pos = 0;
            data_0207efac.index = 0;
            data_0207efac.ownId = 0;
        } else {
            UnkFSArchive_0200b20c *p_tail = data_0207efa8;
            while (p_tail->next) {
                p_tail = p_tail->next;
            }
            p_tail->next = p_arc;
            p_arc->prev = p_tail;
        }
        p_arc->name = func_0200b8cc(name, name_len);
        p_arc->flag |= 1;
        ret = TRUE;
    }
    (void)OS_RestoreInterrupts(bak_intr);
    return ret;
}
