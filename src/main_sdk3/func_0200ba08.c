#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    vu32 stat;   /* 0x0c */
    u8 pad10[4];
    s32 error;   /* 0x14 */
    u16 queue;   /* 0x18 */
} UnkFSFile_0200ba08;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_02005c48(void *queue);
BOOL func_0200b514(UnkFSFile_0200ba08 *f);

static inline BOOL func_0200ba08_IsBusy(UnkFSFile_0200ba08 *f)
{
    return (f->stat & 1) ? TRUE : FALSE;
}

BOOL func_0200ba08(UnkFSFile_0200ba08 *p_file)
{
    BOOL is_owner = FALSE;
    u32 bak_psr = OS_DisableInterrupts();

    if (func_0200ba08_IsBusy(p_file) != FALSE) {
        is_owner = !(p_file->stat & 0x44);
        if (is_owner) {
            p_file->stat |= 4;
            do {
                func_02005c48(&p_file->queue);
            } while (!(p_file->stat & 0x40));
        } else {
            do {
                func_02005c48(&p_file->queue);
            } while (func_0200ba08_IsBusy(p_file) != FALSE);
        }
    }
    (void)OS_RestoreInterrupts(bak_psr);
    if (is_owner) {
        return func_0200b514(p_file);
    }
    return (p_file->error == 0) ? TRUE : FALSE;
}
