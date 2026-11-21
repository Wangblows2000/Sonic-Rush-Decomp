#include <nitro/types.h>

void ExtraBoss_SnapObjToCameraX(void *p);
u32 *ExtraBoss_GetWork(void);

void ExtraBossObj_State_SuctionFx(u8 *p) {
    ExtraBoss_SnapObjToCameraX(p);
    if (!(*ExtraBoss_GetWork() & 0x40)) {
        *(u32 *)(p + 0x14) |= 8;
    }
}
