#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 lockId;
} UnkStruct_02080f54;

extern UnkStruct_02080f54 data_02080f54;

void CpuSet(const void *src, void *dst, u32 ctrl);
s32 OS_GetLockID(void);

void CTRDGi_InitCommon(void)
{
    u32 zero = 0;
    CpuSet(&zero, &data_02080f54, 0x5000001);
    data_02080f54.lockId = OS_GetLockID();
}
