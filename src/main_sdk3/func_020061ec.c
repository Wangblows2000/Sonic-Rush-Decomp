#include <nitro/types.h>

s32 OS_GetLockID(void);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
u32 OS_ReadOwnerOfLockWord(void *lock);
s32 OS_TryLockCartridge(u16 lockID);
s32 OS_UnlockCartridge(u16 lockID);

u32 func_020061ec(void)
{
    u32 result;
    BOOL done;
    u16 id;
    s32 lockResult;
    u32 enabled;
    done = FALSE;
    id = (u16)OS_GetLockID();
    result = 0;

    do {
        lockResult = -1;
        enabled = OS_DisableInterrupts();
        if ((OS_ReadOwnerOfLockWord((void *)0x027fffe8) & 0x40) || (lockResult = OS_TryLockCartridge(id)) == 0) {
            if (*(u32 *)0x08000000 == 0x544e494e && *(u32 *)0x08000004 == 0x4f444e45) {
                result = 0x01000000;
            } else {
                result = 0x02000000;
            }
            if (lockResult == 0) {
                OS_UnlockCartridge(id);
                done = TRUE;
            }
        }
        (void)OS_RestoreInterrupts(enabled);
    } while (!done);
    return result;
}
