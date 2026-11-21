#include <nitro/types.h>
#include <NitroSDK/blk.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OS_Terminate(void);

void MBi_AttachCacheBuffer(UnkStruct_0201a16c *s, u32 base, u32 size, u8 *ptr, u32 state)
{
    u32 e = OS_DisableInterrupts();
    UnkBlock_0201a16c *b;

    for (b = &s->blocks[0];; b++) {
        if (b >= &s->blocks[4]) {
            OS_Terminate();
        }
        if (b->state == 0) {
            b->base = base;
            b->size = size;
            b->ptr = ptr;
            b->state = state;
            break;
        }
    }
    OS_RestoreInterrupts(e);
}
