#include <nitro/types.h>

void MI_CpuFill8(void *dst, u8 v, u32 size);
extern u8 *data_02080f44;
void MBi_ClearParentPieceBuffer(s32 i) {
    s32 n;
    if (data_02080f44 == 0) return;
    n = i - 1;
    MI_CpuFill8(data_02080f44 + n * 32, 0, 0x1e);
    *(u32 *)(data_02080f44 + n * 4 + 0x1e0) = 0;
}
