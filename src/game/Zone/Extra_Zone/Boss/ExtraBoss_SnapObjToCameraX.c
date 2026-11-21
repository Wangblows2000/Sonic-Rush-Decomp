#include <nitro/types.h>

u8 *ExtraBoss_GetWork(void);

void ExtraBoss_SnapObjToCameraX(u8 *p) {
    *(s32 *)(p + 0x18) = *(s32 *)(ExtraBoss_GetWork() + 0x31c);
}
