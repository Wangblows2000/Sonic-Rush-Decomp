#include <nitro/types.h>

typedef struct {
    u32 lockFlag;
    u16 ownerID;
} UnkLockWord;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 OS_DisableInterrupts_IrqAndFiq(void);
u32 OS_RestoreInterrupts_IrqAndFiq(u32 e);

s32 OSi_DoUnlockByWord(u16 lockId, UnkLockWord *lock, void (*ctrl)(void), BOOL disableFiq)
{
    u32 e;
    if (lockId != lock->ownerID) {
        return -2;
    }
    if (disableFiq) {
        e = OS_DisableInterrupts_IrqAndFiq();
    } else {
        e = OS_DisableInterrupts();
    }
    lock->ownerID = 0;
    if (ctrl != NULL) {
        ctrl();
    }
    lock->lockFlag = 0;
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(e);
    } else {
        OS_RestoreInterrupts(e);
    }
    return 0;
}
