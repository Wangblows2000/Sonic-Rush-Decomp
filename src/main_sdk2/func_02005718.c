#include <nitro/types.h>

typedef struct {
    u32 lockFlag;
    u16 ownerID;
} UnkLockWord;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 OS_DisableInterrupts_IrqAndFiq(void);
u32 OS_RestoreInterrupts_IrqAndFiq(u32 e);
s32 func_02008640(u32 v, UnkLockWord *lock);

s32 func_02005718(u16 lockId, UnkLockWord *lock, void (*ctrl)(void), BOOL disableFiq)
{
    u32 e;
    s32 prev;
    if (disableFiq) {
        e = OS_DisableInterrupts_IrqAndFiq();
    } else {
        e = OS_DisableInterrupts();
    }
    prev = func_02008640(lockId, lock);
    if (prev == 0) {
        if (ctrl != NULL) {
            ctrl();
        }
        lock->ownerID = lockId;
    }
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(e);
    } else {
        OS_RestoreInterrupts(e);
    }
    return prev;
}
