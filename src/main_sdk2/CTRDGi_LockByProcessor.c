#include <nitro/types.h>

typedef struct {
    u32 locked;
    u32 irq;
} UnkStruct_0201aa68;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 OS_ReadOwnerOfLockWord(void *lock);
s32 OS_TryLockCartridge(u16 lockId);
void WaitByLoop(s32 count);

void CTRDGi_LockByProcessor(u16 lockId, UnkStruct_0201aa68 *s)
{
    for (;;) {
        s->irq = OS_DisableInterrupts();
        s->locked = OS_ReadOwnerOfLockWord((void *)0x027fffe8) & 0x40;
        if (s->locked) {
            return;
        }
        if (OS_TryLockCartridge(lockId) == 0) {
            return;
        }
        OS_RestoreInterrupts(s->irq);
        WaitByLoop(1);
    }
}
