#include <nitro/types.h>

void *OS_GetArenaLo(u32 a);
extern u16 gTransferJobUsed;
extern void *gTransferJobFreeList[];

void TransferJob_Free(void *p) {
    if (OS_GetArenaLo(0) == p) return;
    gTransferJobUsed--;
    gTransferJobFreeList[gTransferJobUsed] = p;
}
