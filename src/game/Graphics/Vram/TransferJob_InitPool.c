#include <nitro/types.h>

typedef struct {
    u8 data[0x20];
} UnkBlock_02037efc;

extern u16 gTransferJobUsed;
extern UnkBlock_02037efc gTransferJobPool[];
extern UnkBlock_02037efc *gTransferJobFreeList[];

void MIi_CpuClear32(u32 data, void *dst, u32 size);

void TransferJob_InitPool(void) {
    s32 i;
    {
        vu32 zero = 0;
        gTransferJobUsed = 0;
        MIi_CpuClear32(zero, gTransferJobPool, sizeof(UnkBlock_02037efc) * 256);
    }
    for (i = 0; i < 256; i++) {
        gTransferJobFreeList[i] = &gTransferJobPool[i];
    }
}
