#include <nitro/types.h>

void *func_02006abc(u32 a);
extern u16 gTransferJobUsed;
extern void *gTransferJobFreeList[];

void TransferJob_Free(void *p) {
    if (func_02006abc(0) == p) return;
    gTransferJobUsed--;
    gTransferJobFreeList[gTransferJobUsed] = p;
}
