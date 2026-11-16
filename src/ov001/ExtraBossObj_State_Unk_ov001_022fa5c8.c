#include <nitro/types.h>

void ExtraBoss_Unk_ov001_022fa558(void *p);
u32 *ExtraBoss_GetWork(void);

void ExtraBossObj_State_Unk_ov001_022fa5c8(u8 *p) {
    ExtraBoss_Unk_ov001_022fa558(p);
    if (!(*ExtraBoss_GetWork() & 0x40)) {
        *(u32 *)(p + 0x14) |= 8;
    }
}
