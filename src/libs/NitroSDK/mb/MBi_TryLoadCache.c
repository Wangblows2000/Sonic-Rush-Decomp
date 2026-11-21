#include <nitro/types.h>
#include <NitroSDK/blk.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

BOOL MBi_TryLoadCache(UnkStruct_0201a16c *s, u32 addr, void *dest, u32 size)
{
    BOOL ret = FALSE;
    u32 e = OS_DisableInterrupts();
    UnkBlock_0201a16c *b;

    for (b = &s->blocks[0]; b < &s->blocks[4]; b++) {
        if (b->state >= 2) {
            s32 off = addr - b->base;
            if (off >= 0 && off + size <= b->size) {
                MI_CpuCopy8(b->ptr + off, dest, size);
                ret = TRUE;
                s->unk0 = TRUE;
                break;
            }
        }
    }
    OS_RestoreInterrupts(e);
    return ret;
}
