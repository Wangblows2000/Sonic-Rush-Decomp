#include <nitro/types.h>

typedef struct { u32 lockFlag; u16 ownerID; u16 extension; } OSLockWord;
u16 OS_ReadOwnerOfLockWord(volatile OSLockWord *lock) { return lock->ownerID; }
